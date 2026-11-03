from pathlib import Path
p=Path('X:/y5linux/phone/ui/src/render.c');s=p.read_text(encoding="utf-8")
a=s.index('static void glass(');b=s.index('static void wallpaper_paint',a)
s=s[:a]+'''/* CPU-friendly glass tint/highlight; backdrop blur/refraction belongs to compositor. */
static void glass(cairo_t*c,double x,double y,double w,double h,double r){
    rr(c,x,y,w,h,r);rgba(c,.10,.10,.11,.92);cairo_fill_preserve(c);
    cairo_pattern_t*p=cairo_pattern_create_linear(x,y,x,y+h);
    cairo_pattern_add_color_stop_rgba(p,0,1,1,1,.13);
    cairo_pattern_add_color_stop_rgba(p,.18,1,1,1,.035);
    cairo_pattern_add_color_stop_rgba(p,1,1,1,1,.045);
    cairo_set_source(c,p);cairo_set_line_width(c,.7);cairo_stroke(c);cairo_pattern_destroy(p);
}
'''+s[b:]
a=s.index('static void settings_content(');b=s.index('static void switcher(',a)
s=s[:a]+'''static void settings_content(cairo_t*c,double x,double y,double w,double h,const struct mfw_model*m){
    cairo_save(c);cairo_rectangle(c,x,y,w,h);cairo_clip(c);
    rgba(c,0,0,0,1);cairo_paint(c);
    rgba(c,1,1,1,.96);text(c,"Settings",x+16,y+12,18,true,(int)w-32,false);
    /* Compact desktop density, with grouped cards inspired by owner's references. */
    double top=y+54,cardw=w-24;
    rr(c,x+12,top,cardw,94,19);rgba(c,.115,.115,.12,1);cairo_fill(c);
    glass(c,x+24,top+14,32,32,9);rgba(c,.75,.76,.80,1);mfw_icon(c,ICON_GEAR,x+29,top+19,22,22);
    rgba(c,1,1,1,.96);text(c,"MICROFW",x+66,top+15,16,true,(int)cardw-80,false);
    rgba(c,.58,.58,.61,1);text(c,"Device and system",x+66,top+38,12,false,(int)cardw-80,false);
    text(c,"Live hardware status",x+24,top+67,11,false,(int)cardw-24,false);
    double rows=top+114,rowh=44;
    rr(c,x+12,rows,cardw,4*rowh,18);rgba(c,.115,.115,.12,1);cairo_fill(c);
    char v[96];const char*labels[]={"Battery","Temperature","Voltage","Processor"};
    for(int i=0;i<4;i++){
        double yy=rows+i*rowh;
        if(i){rgba(c,1,1,1,.12);cairo_rectangle(c,x+56,yy,cardw-56,.6);cairo_fill(c);}
        glass(c,x+24,yy+11,24,24,7);
        rgba(c,i==0?.20:.62,i==0?.84:.64,i==0?.40:.69,1);
        if(i==0){rr(c,x+29,yy+19,13,8,2);cairo_set_line_width(c,1.4);cairo_stroke(c);}
        else{cairo_arc(c,x+36,yy+23,5,0,6.283185307);cairo_set_line_width(c,1.4);cairo_stroke(c);}
        rgba(c,.95,.95,.97,1);text(c,labels[i],x+58,yy+14,13,false,(int)(w*.43)-48,false);
        if(i==0)snprintf(v,sizeof v,"%s",m->battery_status);
        if(i==1){if(m->temp_deci_c>=0)snprintf(v,sizeof v,"%.1f °C",m->temp_deci_c/10.0);else snprintf(v,sizeof v,"—");}
        if(i==2){if(m->voltage_uv>0)snprintf(v,sizeof v,"%.3f V",m->voltage_uv/1000000.0);else snprintf(v,sizeof v,"—");}
        if(i==3){if(m->cpu_hz)snprintf(v,sizeof v,"%.0f MHz",m->cpu_hz/1000000.0);else snprintf(v,sizeof v,"—");}
        rgba(c,.57,.57,.60,1);text(c,v,x+w*.46,yy+14,12,false,(int)(w*.54)-28,false);
    }
    rgba(c,.48,.48,.51,1);text(c,"Unavailable readings remain unknown.",x+24,rows+4*rowh+16,11,false,(int)cardw-24,false);
    cairo_restore(c);
}
'''+s[b:]
p.write_text(s,encoding='utf-8',newline='\n')
Path('D:/y5_linux_work/microfw-ui/src/render.c').write_text(s,encoding='utf-8',newline='\n')

