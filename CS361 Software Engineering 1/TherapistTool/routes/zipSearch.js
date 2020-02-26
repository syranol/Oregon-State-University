module.exports = function(){
    var express = require('express');
    var router = express.Router();

    router.get('/zipSearch',function(req,res){
        res.render('zipSearch');
      });

    /*Searches for a therapist by their zip code*/
    router.post('/zipSearch', function(req, res){
        var context = {};
        var mysql = req.app.get('mysql');
        var sql = "SELECT concat(t.fname, ' ', t.lname) AS Full_Name, s.name AS Specialty, so.name AS Sexual_Orientation, r.name AS Religion, t.exp AS Experience, l.zip AS Zip, t.mail AS Email FROM therapists t INNER JOIN `specialty` s ON t.spec = s.specID INNER JOIN `sexualOrientation` so ON t.sOrien = so.sexID INNER JOIN `religion` r ON t.rel = r.relID INNER JOIN `location` l ON t.loc = l.locID WHERE l.zip = ?";
        var inserts = [req.body.zip];
        sql = mysql.pool.query(sql,inserts,function(error, results, fields){
            if(error){
                res.write(JSON.stringify(error));
                res.end();
            }else{
                context.therapists = results;
                context.zip = [req.body.zip];
                res.render('zipSearch', context);
            }
        });
    });

    return router;
}();


/* DMQ

SELECT concat(t.fname, ' ', t.lname) AS Full_Name, s.name AS Specialty, so.name AS Sexual_Orientation, r.name AS Religion, t.exp AS Experience, t.mail AS Email, l.zip AS Zip 
FROM `therapists` t 
INNER JOIN `specialty` s ON t.spec = s.specID 
INNER JOIN `sexualOrientation` so ON t.sOrien = so.sexID  
INNER JOIN `religion` r ON t.rel = r.relID 
INNER JOIN `location` l ON t.loc = l.locID 
ORDER BY t.therID 
WHERE l.zip = ?

SELECT patID AS Patient_ID, p.fname AS First_Name, p.lname AS Last_Name, pnum AS Phone_Number, i.name AS Insurance, DATE_FORMAT(txdate, '%M %e %Y') AS Treatment_Date, concat('Dr. ', ph.fname, ' ', ph.lname) AS Physician 
FROM `patients` p 
INNER JOIN `physicians` ph ON p.doctor = ph.physID 
INNER JOIN `insurance` i ON p.ins = i.insID 
WHERE p.lname = ?

SELECT concat(t.fname, ' ', t.lname) AS Full_Name, s.name AS Specialty, so.name AS Sexual_Orientation, r.name AS Religion, t.exp AS Experience, t.mail AS Email, l.zip AS Zip 
FROM `therapists` t 
INNER JOIN `specialty` s ON t.spec = s.specID 
INNER JOIN `sexualOrientation` so ON t.sOrien = so.sexID  
INNER JOIN `religion` r ON t.rel = r.relID 
INNER JOIN `location` l ON t.loc = l.locID 
ORDER BY t.therID 
WHERE l.zip = ?

SELECT 
concat(t.fname, ' ', t.lname) AS Full_Name, 
s.name AS Specialty,
so.name AS Sexual_Orientation,
r.name AS Religion,
t.exp AS Experience,
l.zip AS Zip,
t.mail AS Email
FROM therapists t
INNER JOIN `specialty` s ON t.spec = s.specID
INNER JOIN `sexualOrientation` so ON t.sOrien = so.sexID
INNER JOIN `religion` r ON t.rel = r.relID
INNER JOIN `location` l ON t.loc = l.locID
WHERE l.zip = 97202

SELECT concat(t.fname, ' ', t.lname) AS Full_Name, s.name AS Specialty, so.name AS Sexual_Orientation, r.name AS Religion, t.exp AS Experience, l.zip AS Zip, t.mail AS Email FROM therapists t INNER JOIN `specialty` s ON t.spec = s.specID INNER JOIN `sexualOrientation` so ON t.sOrien = so.sexID INNER JOIN `religion` r ON t.rel = r.relID INNER JOIN `location` l ON t.loc = l.locID WHERE l.zip = ?

SELECT concat(t.fname, ' ', t.lname) AS Full_Name, s.name AS Specialty, so.name AS Sexual_Orientation, r.name AS Religion, t.exp AS Experience, t.mail AS Email, l.zip AS Zip 
FROM `therapists` t 
INNER JOIN `specialty` s ON t.spec = s.specID 
INNER JOIN `sexualOrientation` so ON t.sOrien = so.sexID  
INNER JOIN `religion` r ON t.rel = r.relID 
INNER JOIN `location` l ON t.loc = l.locID 
ORDER BY t.therID 

SELECT concat(t.fname, ' ', t.lname) AS Full_Name, s.name AS Specialty, so.name AS Sexual_Orientation, r.name AS Religion, t.exp AS Experience, t.mail AS Email FROM `therapists` t INNER JOIN specialty s ON t.spec = s.specID INNER JOIN sexualOrientation so ON t.sOrien = so.sexID  INNER JOIN religion r ON t.rel = r.name INNER JOIN location l ON t.loc = l.locID ORDER BY t.therID WHERE l.zip = ?
*/




