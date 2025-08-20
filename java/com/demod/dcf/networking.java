package com.demod.dcf.networking;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.stub.StreamObserver;
import com.demod.dcf.generated.DCFServiceGrpc;
import com.demod.dcf.generated.DCFMessage;

public class Networking {
    private final ManagedChannel channel;
    private final DCFServiceGrpc.DCFServiceBlockingStub stub;
    private final String mode;

    public Networking(String host, int port, String mode) {
        this.channel = ManagedChannelBuilder.forAddress(host, port).usePlaintext().build();
        this.stub = DCFServiceGrpc.newBlockingStub(channel);
        this.mode = mode;
    }

    public String send(String data, String recipient) {
        DCFMessage request = DCFMessage.newBuilder().setData(data).setRecipient(recipient).build();
        DCFMessage response = stub.sendMessage(request);
        return response.getData();
    }

    // Add stream for receive: stub.receiveStream(Empty.newBuilder().build(), observer);
    public void shutdown() { channel.shutdown(); }
}
