module Main where

import DCF.Networking
import DCF.Redundancy (healthCheck)  -- Hypothetical redundancy module
import System.UUID.V4 (uuid)

main :: IO ()
main = do
  net <- newNetworking "localhost" 50051 "p2p"
  resp <- send net "P2P Hello!" "peer1"
  case resp of
    Right r -> putStrLn $ "Response: " ++ show r
    Left e  -> print e
  rtt <- healthCheck net "peer1"
  putStrLn $ "RTT: " ++ show rtt ++ " ms"
