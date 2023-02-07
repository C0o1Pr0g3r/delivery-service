SELECT mailing.id,
       mailing.wb_num AS waybill_number,
       iif(client.indv_id IS NOT NULL, indv.lastname || ' ' || substr(indv.firstname, 1, 1) || '.' || substr(indv.patronymic, 1, 1) || '.', org.title) AS sender,
       iif(client_1.indv_id IS NOT NULL, indv_1.lastname || ' ' || substr(indv_1.firstname, 1, 1) || '.' || substr(indv_1.patronymic, 1, 1) || '.', org_1.title) AS recipient,
       'Отеделение №' || ofc.num || ' | ' || loc_tp.abbrname || ' ' || loc.title || ', ' || st_tp.abbrname || ' ' || st.title || ', ' || addr.hse_num AS dispatch_point,
       'Отеделение №' || ofc_1.num || ' | ' || loc_tp_1.abbrname || ' ' || loc_1.title || ', ' || st_tp_1.abbrname || ' ' || st_1.title || ', ' || addr_1.hse_num AS arrival_point,
       mailing_loc.descr AS cur_loc
  FROM mailing
       INNER JOIN
       client ON mailing.sender_id = client.id
       LEFT JOIN
       indv ON client.indv_id = indv.id
       LEFT JOIN
       org ON client.org_id = org.id
       INNER JOIN
       client AS client_1 ON mailing.recipient_id = client_1.id
       LEFT JOIN
       indv AS indv_1 ON client_1.indv_id = indv_1.id
       LEFT JOIN
       org AS org_1 ON client_1.org_id = org_1.id
       INNER JOIN
       ofc
       INNER JOIN
       addr
       INNER JOIN
       st
       INNER JOIN
       st_tp
       INNER JOIN
       loc
       INNER JOIN
       loc_tp ON mailing.ofc_whence_id = ofc.id AND 
                 ofc.addr_id = addr.id AND 
                 addr.st_id = st.id AND 
                 st.tp_id = st_tp.id AND 
                 st.loc_id = loc.id AND 
                 loc.tp_id = loc_tp.id
       INNER JOIN
       ofc AS ofc_1
       INNER JOIN
       addr AS addr_1
       INNER JOIN
       st AS st_1
       INNER JOIN
       st_tp AS st_tp_1
       INNER JOIN
       loc AS loc_1
       INNER JOIN
       loc_tp AS loc_tp_1 ON mailing.ofc_where_id = ofc_1.id AND 
                             ofc_1.addr_id = addr_1.id AND 
                             addr_1.st_id = st_1.id AND 
                             st_1.tp_id = st_tp_1.id AND 
                             st_1.loc_id = loc_1.id AND 
                             loc_1.tp_id = loc_tp_1.id
       INNER JOIN
       mailing_loc ON mailing.cur_loc_id = mailing_loc.id
       INNER JOIN
       shp ON shp.id = shp_cont.shp_id
       INNER JOIN
       shp_cont ON mailing.id = shp_cont.mailing_id
 WHERE shp.DAT_cmplt IS NULL AND 
       mailing.ofc_whence_id = %1 AND 
       mailing.ofc_where_id = %2
