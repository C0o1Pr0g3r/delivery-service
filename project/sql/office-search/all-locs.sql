SELECT loc.id,
       loc_tp.abbrname || ' ' || loc.title AS title
  FROM loc
       INNER JOIN
       loc_tp ON loc.tp_id = loc_tp.id
       INNER JOIN
       dist ON loc.dist_id = dist.id
 WHERE dist.rgn_id < 12
