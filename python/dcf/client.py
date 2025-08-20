from .config import DCFConfig
from .networking import DCFNetworking
from .redundancy import DCFRedundancy
from .plugin_manager import DCFPluginManager
from .serialization import serialize_message, deserialize_message
from .error import DCFError

class DCFMode:
    CLIENT = "client"
    SERVER = "server"
    P2P = "p2p"
    AUTO = "auto"
    MASTER = "master"

class DCFClient:
    def __init__(self, config_path):
        self.config = DCFConfig.load(config_path)
        self.networking = DCFNetworking(self.config)
        self.redundancy = DCFRedundancy(self.config, self.networking)
        self.plugin_mgr = DCFPluginManager(self.config)
        self.running = False
        self.log_level = 1  # info
        self.current_mode = self.config.mode

    def start(self):
        if self.running: raise DCFError("Already running")
        self.running = True
        self.networking.start(self.current_mode)
        self.redundancy.start(self.current_mode)

    def stop(self):
        if not self.running: raise DCFError("Not running")
        self.running = False
        self.networking.stop()
        self.redundancy.stop()

    def send_message(self, data, recipient):
        serialized = serialize_message(data, self.config.node_id, recipient)
        target = recipient
        if self.current_mode in [DCFMode.P2P, DCFMode.AUTO]:
            target = self.redundancy.get_optimal_route(recipient)
        transport = self.plugin_mgr.get_transport()
        if transport:
            transport.send(serialized, target)
            response_data = transport.receive()
            response = deserialize_message(response_data)
            return response
        else:
            response = self.networking.send(serialized, target)
            return response

    def receive_message(self):
        transport = self.plugin_mgr.get_transport()
        if transport:
            data = transport.receive()
        else:
            data = self.networking.receive()
        if self.current_mode == DCFMode.AUTO:
            # Parse for master commands
            json = self.parse_json(data)
            if json and 'command' in json:
                if json['command'] == 'set_role':
                    self.set_mode(json['role'])
                elif json['command'] == 'update_config':
                    self.config.update(json['key'], json['value'])
        return deserialize_message(data)

    def set_mode(self, mode):
        self.current_mode = mode
        # Reconfigure networking and redundancy
        self.networking.start(mode)
        self.redundancy.start(mode)

    def set_log_level(self, level):
        self.log_level = level
        # Apply log level (placeholder)

    def __del__(self):
        self.stop()
        del self.config
        del self.networking
        del self.redundancy
        del self.plugin_mgr
