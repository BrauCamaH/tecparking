'use strict';
const bodyParser = require('body-parser');
const cors = require('cors');
const morgan = require('morgan');

const express = require('express');
const socketio = require('socket.io');
const http = require('http');
const path = require('path');

const app = express(); //instancia de express
const server = http.createServer(app); //creando el server con http y express como handle request
const io = socketio(server); //iniciando el server de socket.io

app.use(morgan('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(cors());

app.use(express.static(path.join(__dirname, 'public')));

io.on('connection', function(socket) {
  console.log(`client: ${socket.id}`);
  //enviando numero aleatorio cada dos segundo al cliente

  socket.emit('rfid', Math.random());
  //recibiendo el numero aleatorio del cliente
  socket.on('client/random', num => {
    console.log(num);
  });
});

io.on('connection', function(socket) {
  console.log('a user connected');
  socket.on('chat message', function(msg) {
    console.log('message: ' + msg);
  });
  socket.on('disconnect', function() {
    console.log('user disconnected');
  });
});

app.use((req, res, next) => {
  const error = new Error('Not found');
  error.status = 404;
  next(error);
});

app.use((error, req, res, next) => {
  res.status(error.status || 500);
  res.json({
    error: {
      message: error.message,
    },
  });
});

// Start server
const PORT = process.env.PORT || 8080;
server.listen(PORT, () => {
  console.log(`Server running in http://localhost:${PORT}`);
});
