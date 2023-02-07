SELECT mailing.id,
       mailing.wb_num
  FROM mailing
       LEFT JOIN
       stg ON mailing.id = stg.mailing_id
 WHERE stg.mailing_id IS NULL;
