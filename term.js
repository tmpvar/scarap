var serialport = require('serialport');
var repl = require('repl');
var split = require('split');
var spawn = require('child_process').spawn;
var fs = require('fs');

var gcode = fs.readFileSync(__dirname + '/test.gcode').toString().trim().split('\n');
console.log(gcode);
var make = spawn('make', ['clean', 'flash']);
make.stdout.pipe(process.stdout);
make.stderr.pipe(process.stderr);

make.on('close', function(code) {
  if (code !== 0) {
    process.exit();
  }

  serialport.list(function(e, array) {
    array.forEach(function(port) {
      if (port.manufacturer.indexOf('Arduino') > -1) {

        var sp = new (serialport.SerialPort)(port.comName)
        sp.on('open', function() {
          console.log('opened port', port.comName);

          var started = false
          var r;
          sp.on('data', process.stdout.write.bind(process.stdout));
          sp.pipe(split()).on('data', function(line) {
            if (!started && line.indexOf('scarap') > -1) {
              started = true;
              r = repl.start({
                prompt: '> ',
                input: process.stdin,
                output: process.stdout,
                ignoreUndefined : true,
                eval : function(d, context, filename, cb) {
                  d = d.replace(/[\(\)]/g, '');
                  sp.write(d);
                  cb();
                }
              });
              if (gcode.length) {
                var where = 0, dir = 1;
                var timer = setInterval(function() {
                  var line = gcode[where];
                  sp.write(line+'\n');

                  where+=dir;
                  if (where>=gcode.length-1) {
                    dir = -1;
                  } else if (where < 1) {
                    dir = 1;
                  }
                }, 200);
              }

            } else if (started) {
              //process.stdout.write(line + '\n');
            }
          })
        });
        
      }
    })
  });
});