SELECT staff.id,
       staff.lastname || ' ' || staff.firstname || ' ' || staff.patronymic || ' (' || post.title || ')'
  FROM staff
       INNER JOIN
       post ON staff.post_id = post.id
 WHERE (staff.post_id = 3 OR 
        staff.post_id = 4) AND 
       ofc_id = %1;
