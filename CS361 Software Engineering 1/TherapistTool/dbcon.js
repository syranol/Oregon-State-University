var mysql = require('mysql');
var pool = mysql.createPool({
  connectionLimit : 10,
  host            : 'classmysql.engr.oregonstate.edu',
  user            : 'cs361_hollingx',
  password        : '7635',
  database        : 'cs361_hollingx'
});

module.exports.pool = pool;
