SELECT mailing.id,
       mailing.wb_num
  FROM mailing
 WHERE mailing.ofc_whence_id = %1 OR 
       mailing.ofc_where_id = %2;
