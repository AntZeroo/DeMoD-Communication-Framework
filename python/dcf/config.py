import json
from .error import DCFError

class DCFConfig:
    @classmethod
    def load(cls, path):
        with open(path, 'r') as f:
            data = json.load(f)
        instance = cls()
        instance.mode = data.get('mode', 'auto')
        instance.node_id = data.get('node_id', str(uuid.uuid4()))
        instance.peers = data.get('peers', [])
        instance.host = data.get('host', 'localhost')
        instance.port = data.get('port', 50051)
        instance.rtt_threshold = data.get('rtt_threshold', 50)
        instance.plugin_path = data.get('plugins', {}).get('transport')
        return instance

    def update(self, key, value):
        if key == 'mode':
            self.mode = value
        elif key == 'node_id':
            self.node_id = value
        elif key == 'host':
            self.host = value
        elif key == 'port':
            self.port = int(value)
        elif key == 'rtt_threshold':
            self.rtt_threshold = int(value)
        elif key == 'plugin_path':
            self.plugin_path = value
        else:
            raise DCFError("Invalid config key")
