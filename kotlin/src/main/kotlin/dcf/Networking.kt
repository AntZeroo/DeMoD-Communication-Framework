// kotlin/src/main/kotlin/dcf/Networking.kt
import io.grpc.ManagedChannel
import io.grpc.ManagedChannelBuilder
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.flow
import dcf.generated.DCFMessage
import dcf.generated.DCFServiceGrpcKt

class Networking(
    private val host: String,
    private val port: Int,
    private val mode: Mode = Mode.P2P
) {
    private val channel: ManagedChannel = ManagedChannelBuilder.forAddress(host, port).usePlaintext().build()
    private val stub = DCFServiceGrpcKt.dCFServiceCoroutineStub(channel)

    suspend fun sendMessage(data: String, recipient: String): String {
        val request = DCFMessage.newBuilder().setData(data).setRecipient(recipient).build()
        val response = stub.sendMessage(request)
        return response.data
    }

    fun receiveStream(): Flow<DCFMessage> = flow {
        val response = stub.receiveStream(Unit)  // Assuming Empty request
        response.collect { emit(it) }
    }

    fun close() {
        channel.shutdown()
    }
}
