SELECT staff.id,
       staff.lastname || ' ' || staff.firstname || ' ' || staff.patronymic,
       post.title,
       ofc.id AS ofc_id,
       ofc.num AS ofc_num,
       ofc_tp.descr AS ofc_tp,
       loc_tp.abbrname || ' ' || loc.title AS ofc_loc,
       loc_tp.abbrname || ' ' || loc.title || ', ' || st_tp.abbrname || ' ' || st.title || ', ' || addr.hse_num AS address
  FROM staff
       INNER JOIN
       post ON staff.post_id = post.id
       INNER JOIN
       ofc ON staff.ofc_id = ofc.id
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
 WHERE staff.post_id = 1 AND 
       staff.id = %1;
