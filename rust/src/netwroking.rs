// src/networking.rs
use tonic::{transport::Channel, Request, Response, Status};
use crate::proto::{DcfMessage, dcf_service_client::DcfServiceClient, dcf_service_server::{DcfService, DcfServiceServer}};

#[derive(Clone)]
pub struct Networking {
    client: Option<DcfServiceClient<Channel>>,
    mode: String,
}

impl Networking {
    pub async fn new(host: &str, port: u16, mode: &str) -> Self {
        let client = DcfServiceClient::connect(format!("http://{}:{}", host, port)).await.ok();
        Self { client, mode: mode.to_string() }
    }

    pub async fn send(&mut self, msg: DcfMessage) -> Result<DcfMessage, Status> {
        if let Some(client) = &mut self.client {
            let response = client.send_message(Request::new(msg)).await?;
            Ok(response.into_inner())
        } else {
            Err(Status::unavailable("No client"))
        }
    }
}

#[tonic::async_trait]
impl DcfService for Networking {
    async fn send_message(&self, request: Request<DcfMessage>) -> Result<Response<DcfMessage>, Status> {
        // Handle server-side logic
        let reply = DcfMessage { data: format!("Echo: {}", request.into_inner().data), ..Default::default() };
        Ok(Response::new(reply))
    }
}

// Server startup example
pub async fn start_server(addr: &str) -> Result<(), Box<dyn std::error::Error>> {
    let net = Networking { client: None, mode: "server".to_string() };
    Server::builder().add_service(DcfServiceServer::new(net)).serve(addr.parse()?).await?;
    Ok(())
}
