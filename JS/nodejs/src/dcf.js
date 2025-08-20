// nodejs/src/dcf.js
const grpc = require('@grpc/grpc-js');
const protoLoader = require('@grpc/proto-loader');
const WebSocket = require('ws');
const uuid = require('uuid');
const packageDefinition = protoLoader.loadSync('services.proto');
const proto = grpc.loadPackageDefinition(packageDefinition).DCFService;

class DCFNetworking {
  constructor(config) {
    this.config = config; // From config.json (mode, host, port, peers, etc.)
    this.mode = config.mode || 'p2p';
    this.plugins = {}; // For dynamic loading
    this.peers = config.peers || [];
    this.client = null;
    this.server = null;
    this.initTransport();
  }

  initTransport() {
    switch (this.config.transport) {
      case 'gRPC':
        this.client = new proto.DCFService(`${this.config.host}:${this.config.port}`, grpc.credentials.createInsecure());
        break;
      case 'WebSocket':
        this.ws = new WebSocket(`ws://${this.config.host}:${this.config.port}`);
        this.ws.on('message', this.handleMessage.bind(this));
        break;
      // Add UDP/TCP via 'dgram' or 'net' modules for Node.js
    }
  }

  send(data, recipient, callback) {
    const msg = { sender: this.config.node_id, recipient, data, timestamp: Date.now(), sequence: uuid.v4() };
    if (this.config.transport === 'gRPC') {
      this.client.SendMessage(msg, (err, response) => callback(err, response ? response.data : null));
    } else if (this.config.transport === 'WebSocket') {
      this.ws.send(JSON.stringify(msg)); // Fallback serialization; use Protobuf for efficiency
      // Handle response via ws.on('message')
    }
  }

  handleMessage(message) {
    // Deserialize (Protobuf or JSON) and process
    console.log('Received:', message);
  }

  // AUTO Mode: Listen for master commands
  setRole(role) {
    this.mode = role;
    // Re-init based on new role (e.g., start server if 'server')
  }

  // P2P Redundancy: RTT Grouping
  async healthCheck(peer) {
    const start = Date.now();
    // Use gRPC health check or ping
    // Return RTT: Date.now() - start
  }

  loadPlugin(name, path) {
    this.plugins[name] = require(path); // Dynamic import
    // Version check: if (this.plugins[name].getVersion() !== '1.0') throw Error;
  }
}

module.exports = DCFNetworking;
