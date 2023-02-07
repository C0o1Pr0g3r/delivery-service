SELECT DISTINCT loc_1.id,
                loc_tp_1.abbrname || ' ' || loc_1.title
  FROM mailing
       INNER JOIN
       ofc ON mailing.ofc_whence_id = ofc.id
       INNER JOIN
       ofc AS ofc_1 ON mailing.ofc_where_id = ofc_1.id
       INNER JOIN
       ofc_tp AS ofc_tp_1 ON ofc_1.tp_id = ofc_tp_1.id
       INNER JOIN
       addr AS addr_1 ON ofc_1.addr_id = addr_1.id
       INNER JOIN
       st AS st_1 ON addr_1.st_id = st_1.id
       INNER JOIN
       st_tp AS st_tp_1 ON st_1.tp_id = st_tp_1.id
       INNER JOIN
       loc AS loc_1 ON st_1.loc_id = loc_1.id
       INNER JOIN
       loc_tp AS loc_tp_1 ON loc_1.tp_id = loc_tp_1.id
       INNER JOIN
       stg ON stg.mailing_id = mailing.id
 WHERE stg.DAT_cmplt IS NULL AND 
       ofc.id = related_office
