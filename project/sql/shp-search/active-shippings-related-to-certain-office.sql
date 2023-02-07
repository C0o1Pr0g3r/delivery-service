SELECT shp.id,
       shp.wb_num
  FROM shp
 WHERE DAT_cmplt IS NULL AND 
       shp.dspch_pt = %1 OR 
       shp.arr_pt = %2;
