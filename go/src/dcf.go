package dcf

import (
	"context"
	"errors"
	"google.golang.org/grpc"
	pb "github.com/ALH477/DeMoD-Communication-Framework/go/src/proto"
)

type Mode string
const (
	ClientMode Mode = "client"
	ServerMode Mode = "server"
	P2PMode    Mode = "p2p"
	AutoMode   Mode = "auto"
	MasterMode Mode = "master"
)

type DCFError string
var (
	ErrConfigInvalid      = errors.New("invalid config")
	ErrMasterUnreachable  = errors.New("master node unreachable")
	// Add more as per SDK guidelines
)

type DCFClient struct {
	conn   *grpc.ClientConn
	stub   pb.DCFServiceClient
	mode   Mode
	config *Config // From config.go
	// Add redundancy, plugin manager fields
}

func NewDCFClient(configPath string) (*DCFClient, error) {
	cfg, err := LoadConfig(configPath)
	if err != nil {
		return nil, err
	}
	conn, err := grpc.Dial(cfg.Host+":"+string(cfg.Port), grpc.WithInsecure())
	if err != nil {
		return nil, err
	}
	return &DCFClient{
		conn:   conn,
		stub:   pb.NewDCFServiceClient(conn),
		mode:   Mode(cfg.Mode),
		config: cfg,
	}, nil
}

// Methods: Start(), Stop(), SendMessage(data string, recipient string) (string, error), etc.
