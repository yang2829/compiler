PROGRAM test2;
CONST
  msg1="x=? ",
  msg2=" to binary = ",
  msg3="x cannot be less than 0!";
VAR
  x,y;
PROCEDURE bin;
  VAR t;
  BEGIN
    y:=0;
    t:=x;
    WHILE t != 0 DO
    BEGIN
    	y:=y*10+t % 2;
    	t:= t \ 2;
    END;
  END;
BEGIN
  WRITE(msg1);
  READ(x);
  IF x>=0 THEN 
  BEGIN
    CALL bin;
    WRITE(x, msg2, y);
  END
  ELSE WRITE(msg3);
END;
