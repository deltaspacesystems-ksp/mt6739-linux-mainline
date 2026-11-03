#include <assert.h>
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
typedef uint8_t u8;
#define IIO_CHAN_INFO_RAW 0
#define IIO_VAL_INT 1
#define dev_err(...) ((void)0)
struct i2c_client {int dev;};
struct c2_data {struct i2c_client *client;int lock;};
struct iio_dev {struct c2_data data;};
struct iio_chan_spec {unsigned address;};
static void *iio_priv(struct iio_dev *d){return &d->data;}
static void mutex_lock(int *p){assert(!*p);*p=1;}
static void mutex_unlock(int *p){assert(*p);*p=0;}
static int calls,failcall,ctrl,writes,slept,mismatch;
static int c2_read(struct i2c_client *c,u8 r,u8 *b,unsigned n){
 (void)c;if(++calls==failcall)return -EIO;
 if(r==0){assert(n==1);*b=mismatch&&calls>=5?2:ctrl;}
 else {assert((r==13||r==10)&&n==3);b[0]=0x56;b[1]=0x34;b[2]=0x12;}
 return 0;
}
static int c2_control(struct i2c_client *c,u8 v){(void)c;assert(v==0||v==2);writes++;if(++calls==failcall)return -EIO;ctrl=v;return 0;}
static void msleep(unsigned ms){assert(ms==500);slept++;}
static int c2_read_raw(struct iio_dev *indio, const struct iio_chan_spec *chan,
                       int *val, int *val2, long mask)
{
 struct c2_data *d = iio_priv(indio);
 u8 ctrl, bytes[3];
 int ret, restore;
 if (mask != IIO_CHAN_INFO_RAW) return -EINVAL;
 mutex_lock(&d->lock);
 ret = c2_read(d->client, 0, &ctrl, 1);
 if (ret) goto out;
 if (ctrl) { ret = -EBUSY; goto out; }
 ret = c2_control(d->client, 2);
 if (!ret) {
  msleep(500);
  ret = c2_read(d->client, chan->address, bytes, 3);
 }
 restore = c2_control(d->client, 0);
 if (!restore) restore = c2_read(d->client, 0, &ctrl, 1);
 if (!restore && ctrl) restore = -EIO;
 if (restore) { dev_err(&d->client->dev, "ALS restore failed: %d\n", restore); ret = restore; }
 if (!ret) { *val = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16); ret = IIO_VAL_INT; }
out:
 mutex_unlock(&d->lock);
 return ret;
}

int main(void){
 struct i2c_client c={0};struct iio_dev d={.data={.client=&c}};struct iio_chan_spec ch={.address=13};int v=0,v2=0;
 assert(c2_read_raw(&d,&ch,&v,&v2,99)==-EINVAL);assert(calls==0);
 for(int fail=0;fail<=5;fail++){
  calls=ctrl=writes=slept=mismatch=0;failcall=fail;v=-1;
  int rc=c2_read_raw(&d,&ch,&v,&v2,IIO_CHAN_INFO_RAW);
  if(!fail){assert(rc==IIO_VAL_INT&&v==0x123456&&ctrl==0&&slept==1);}
  else {assert(rc<0&&v==-1);if(fail!=4)assert(ctrl==0);}
  assert(d.data.lock==0);
 }
 calls=0;ctrl=1;failcall=0;writes=0;assert(c2_read_raw(&d,&ch,&v,&v2,0)==-EBUSY);assert(!writes);
 calls=ctrl=0;mismatch=1;assert(c2_read_raw(&d,&ch,&v,&v2,0)==-EIO);assert(!d.data.lock);
 puts("PASS exact IIO callback: sample decode, ALS restoration, busy rejection, each transaction failure, restore mismatch, lock cleanup");
}
