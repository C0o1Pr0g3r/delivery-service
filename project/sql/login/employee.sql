SELECT staff.id,
       staff.lastname || ' ' || staff.firstname || ' ' || staff.patronymic
  FROM staff
 WHERE staff.post_id = 1;
