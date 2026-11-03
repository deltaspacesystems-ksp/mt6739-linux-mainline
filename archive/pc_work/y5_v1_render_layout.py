"""Selected Y5 v1 single-layer/single-sample geometry facts; no GPU submission."""
import json

def layout(width,height):
 if not 1<=width<=4096 or not 1<=height<=4096: raise ValueError('dimensions outside stock limit')
 x=(width+15)//16;y=(height+15)//16;n=1<<(max(x,y)-1).bit_length()
 return {'width':width,'height':height,'tiles_x':x,'tiles_y':y,'tile_max_x':x-1,'tile_max_y':y-1,'region_bytes_aligned64':(x*y*5+63)&~63,'tail_bytes':n*n*8,'ppp_screen':((height-1)<<16)|(width-1),'te_screen':((y-1)<<12)|(x-1),'grid_candidate':(x<<16)|y,'sample_count':1,'layers':1}
if __name__=='__main__':
 for w,h in [(1,1),(16,16),(17,33),(64,64),(4096,4096)]:
  q=layout(w,h);assert q['region_bytes_aligned64']%64==0;assert q['tail_bytes']>=q['tiles_x']*q['tiles_y']*8
 assert layout(64,64)['tail_bytes']==128
 assert layout(64,64)['region_bytes_aligned64']==128
 print(json.dumps(layout(64,64),indent=2))
