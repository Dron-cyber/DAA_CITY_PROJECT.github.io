CREATE TABLE students_socse (
    roll_no INT PRIMARY KEY,
    student_name VARCHAR(100),
    city VARCHAR(50)
);


INSERT INTO students_socse (roll_no, student_name, city) VALUES
(101, 'Amit Kumar', 'Delhi'),
(102, 'Sneha Reddy', 'Delhi'),
(103, 'Rohit Sharma', 'Mumbai'),
(104, 'Priya Verma', 'Bengaluru'),
(105, 'Kunal Shah', 'Pune'),
(106, 'Meera Singh', 'Mumbai'),
(107, 'Varun Patel', 'Ahmedabad'),
(108, 'Nisha Das', 'Kolkata'),
(109, 'Arjun Nair', 'Delhi'),
(110, 'Divya Shetty', 'Mumbai');


INSERT INTO students_socse (Roll_No, Student_Name, City)
VALUES (101, 'Amit Kumar', 'Delhi');

INSERT INTO students_socse (Roll_No, Student_Name, City)
VALUES (102, 'Sneha Reddy', 'Delhi');

INSERT INTO students_socse (Roll_No, Student_Name, City)
VALUES (103, 'Rohit Sharma', 'Mumbai');

INSERT INTO students_socse (Roll_No, Student_Name, City)
VALUES (104, 'Priya Verma', 'Bengaluru');

INSERT INTO students_socse (Roll_No, Student_Name, City)
VALUES (105, 'Kunal Shah', 'Pune');

INSERT INTO students_socse (Roll_No, Student_Name, City)
VALUES (106, 'Meera Singh', 'Mumbai');

INSERT INTO students_socse(Roll_No, Student_Name, City)
VALUES (107, 'Varun Patel', 'Ahmedabad');

INSERT INTO students_socse (Roll_No, Student_Name, City)
VALUES (108, 'Nisha Das', 'Kolkata');

INSERT INTO students_socse (Roll_No, Student_Name, City)
VALUES (109, 'Arjun Nair', 'Delhi');

INSERT INTO students_socse (Roll_No, Student_Name, City)
VALUES (110, 'Divya Shetty', 'Mumbai');




UPDATE Students_socse
SET City = CASE Roll_No
    WHEN 101 THEN 'Hubli'
    WHEN 102 THEN 'Hubli'
    WHEN 103 THEN 'Bagalkot'
    WHEN 104 THEN 'Mysore'
    WHEN 105 THEN 'Bangalore'
    WHEN 106 THEN 'Dharwad'
    WHEN 107 THEN 'Dharwad'
    WHEN 108 THEN 'Davangere'
    WHEN 109 THEN 'Bangalore'
    WHEN 110 THEN 'Hubli'
END
WHERE Roll_No BETWEEN 101 AND 110;





CREATE TABLE Marks (
    roll_no INT PRIMARY KEY,
    DBMS INT,
    DAA INT,
    ML INT
);

INSERT INTO Marks (roll_no, DBMS, DAA, ML) VALUES (101, 34, 78, 54);
INSERT INTO Marks (roll_no, DBMS, DAA, ML) VALUES (102, 78, 43, 87);
INSERT INTO Marks (roll_no, DBMS, DAA, ML) VALUES (103, 45, 32, 78);
INSERT INTO Marks (roll_no, DBMS, DAA, ML) VALUES (104, 36, 78, 32);
INSERT INTO Marks (roll_no, DBMS, DAA, ML) VALUES (105, 12, 22, 67);
INSERT INTO Marks (roll_no, DBMS, DAA, ML) VALUES (106, 21, 65, 43);
INSERT INTO Marks (roll_no, DBMS, DAA, ML) VALUES (107, 34, 78, 54);
INSERT INTO Marks (roll_no, DBMS, DAA, ML) VALUES (108, 89, 78, 54);
INSERT INTO Marks (roll_no, DBMS, DAA, ML) VALUES (109, 76, 78, 54);
INSERT INTO Marks (roll_no, DBMS, DAA, ML) VALUES (110, 22, 56, 54);

select * from students_socse;
select * from marks;

--FIND MARKS OF THE STUDENT AMIT KUMAR WHO LIVES IN HUBLI
SELECT *
from students_socse s,marks m
where s.city='Hubli' and s.student_name='Amit Kumar'and s.roll_no = m.roll_no;

select *
from marks
where roll_no = (
select roll_no
from students_socse
where student_name = 'Amit Kumar'
and city = 'Hubli'
)
