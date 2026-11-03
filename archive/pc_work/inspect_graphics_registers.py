import xml.etree.ElementTree as E
from pathlib import Path
wanted=['EVENT_PIXEL_PDS_CODE','EVENT_PIXEL_PDS_DATA','EVENT_PIXEL_PDS_INFO','ISP_BGOBJVALS','TE_PSG','PPP_CTRL','PPP_WCLAMP','ISP_RGN','ISP_CTL','ISP_MTILE_SIZE','PDS_BGND3_SIZEINFO']
r=E.parse('/home/y5/mesa-native/src/imagination/csbgen/rogue/cr.xml').getroot()
for node in r:
 if node.get('name') in wanted:
  print(E.tostring(node,encoding='unicode').strip())
