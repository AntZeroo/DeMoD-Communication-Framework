package DCF::Networking;
use Grpc::XS;
use Google::ProtocolBuffers::Dynamic 'DCF::Message';
sub new { ... }  # As in spec
sub send { ... } # Serialize with DCF::Message, call gRPC
sub receive_stream {
    my ($self) = @_;
    return $self->{client}->call('DCFService/ReceiveStream');
}
1;
