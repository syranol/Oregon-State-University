module.exports = function(){
    var express = require('express');
    var router = express.Router();

    function getPeople(res, mysql, context, complete){
        mysql.pool.query("SELECT therID, fname, lname, specialty.name AS spec, exp, location.city AS loc, mail FROM therapists\
        INNER JOIN specialty ON therapists.spec = specialty.specID \
        INNER JOIN location ON therapists.loc = location.locID", 
        function(error, results, fields){
            if(error){
                res.write(JSON.stringify(error));
                res.end();
            }
            context.therapist = results;
            complete();
        });
    }

    function getPerson(res, mysql, context, id, complete){
        var sql = "SELECT therID, fname, lname, specialty.name AS spec, sexualOrientation.name AS sOrien, religion.name AS rel, exp, location.city AS loc, mail FROM therapists\
        INNER JOIN specialty ON therapists.spec = specialty.specID \
        INNER JOIN location ON therapists.loc = location.locID \
        INNER JOIN sexualOrientation ON therapists.sOrien = sexualOrientation.sexID \
        INNER JOIN religion ON therapists.rel = religion.relID WHERE therID = ?";
        var inserts = [id];
        mysql.pool.query(sql, inserts, function(error, results, fields){
            if(error){
                res.write(JSON.stringify(error));
                res.end();
            }
            context.therapist = results[0];
            complete();
        });
    }

    /*Display all therapists. Requires web based javascript to delete users with AJAX*/

    router.get('/', function(req, res){
        var callbackCount = 0;
        var context = {};
        var mysql = req.app.get('mysql');
        getPeople(res, mysql, context, complete);
        function complete(){
            callbackCount++;
            if(callbackCount >= 1){
                res.render('search', context);
            }

        }
    });

    /* Display one therapist */

    router.get('/:id', function(req, res){
        callbackCount = 0;
        var context = {};
        var mysql = req.app.get('mysql');
        getPerson(res, mysql, context, req.params.id, complete);
        function complete(){
            callbackCount++;
            if(callbackCount >= 1){
                res.render('therapist', context);
            }
        }
    });

    return router;
}();