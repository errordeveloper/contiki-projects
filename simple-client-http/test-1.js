var http = require('http'),
    util = require('util');

http.createServer(function (request, response) {

   console.log(util.inspect(request));
   response.writeHead(200, {"Content-Type":"text/plain"});
   response.end ("Hello World!\n");

}).listen(8080, '::0');
