SELECT ofc.id,
       '№' || ofc.num || ' (' || ofc_tp.descr || ')' || ', ' || loc_tp.abbrname || ' ' || loc.title/* || ', ' || st_tp.abbrname || ' ' || st.title || ', ' || addr.hse_num,*/
       /*ofc.addr_id*/
  FROM ofc
       INNER JOIN
       ofc_tp ON ofc.tp_id = ofc_tp.id
       INNER JOIN
       addr ON ofc.addr_id = addr.id
       INNER JOIN
       st ON addr.st_id = st.id
       INNER JOIN
       st_tp ON st.tp_id = st_tp.id
       INNER JOIN
       loc ON st.loc_id = loc.id
       INNER JOIN
       loc_tp ON loc.tp_id = loc_tp.id
