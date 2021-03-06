# SSVM-Proxy

## Project Goal

SSVM-Proxy is a frontend of SSVM to handle requests from SSVMRPC, a RPC handler implementation. When receiving a request from SSVMRPC, SSVM-Proxy will let SSVM execute Wasm bytecode provided by SSVMRPC and resume the program states that are describe in the input JSON file from SSVMRPC. After SSVM has executed the Wasm bytecode, SSVM-Proxy will return the return values and the state of the Wasm bytecode in JSON format.

## Interface of SSVM-Proxy

### SSVM command line interface 

SSVMRPC can use the following command to execute SSVM-Proxy.

```shell
$ SSVM --input_file=/home/johndoe/input.json --output_file=/home/johndoe/output.json --wasm_file=/home/johndoe/bytecode.wasm
```
There are only three parameters:
1. input_file: Put a input JSON file path from SSVMRPC here.
2. output_file: Put a output JSON file path to SSVMRPC here.
3. wasm_file: Put a input JSON file path from SSVMRPC here.

The file formats of three parameters will be mentioned below. 



### Input JSON file: Receive from SSVMRPC

#### Receive a request for executing a Rust program from SSVMRPC
```json
{
    //Debugging Info for Rust Container
    "service_name": "ERC20",  // A string
    "uuid": "0x0000000012345678",  // 64 bits unsigned integer in hex string format
    //Info for SSVM 
    "modules": ["Rust"],
    "execution":
    {
        "function_name": "Mint",  // String format
        "gas": 123, // Integer
        "argument": ["12345678", "87654321"],  // JSON Array for the function's arugments
        "argument_types": ["i64", "i64"],  // Should be i32, i64, f32, or f64
        "return_types": ["i64"],  // JSON Array for function's return type list
        "vm_snapshot": {
            "global" : [
                [0, "0x00000000FFFFFFFF"], [1, "0x00000000FFFFFFFF"]
                // List: [global_id, value_hex_string(64bit)]
            ],  // Global instance
            "memory" : [
                [0, "00000000"]
                // List: [memory_id, memory_dump_hex_string]
            ]   // Memory instance
        } // Dumpped snapshot to restore VM
    }
}
```
#### Receive a request for executing a Ethereum program from SSVMRPC

```json
{
    //Debugging Info for Rust Container
    "service_name": "ERC20",  // A string
    "uuid": "0x12345678",  // 64 bits unsigned integer in hex string format
    //Info for SSVM 
    "modules": ["Ethereum"],
    "execution":
    {
        "function_name": "Mint",  // String format
        "gas": 123,  // Integer
        "argument": ["0x1234", "1000"],  // JSON Array for the function's arugments
        "ethereum": {
            "caller": "0x0",  // 20 bytes hex number in string format
            "call_value": "0x0",  // 32 bytes hex number in string format
            "abi": [{                 // Smart contract ABI
                "constant": true,
                "inputs": [],
                "name": "data",
                "payable": false,
                "type": "function"
            }],
            "storage": {"0000000000000000000000000000000000000000000000000000000000000000":"0000000000000000000000000000000000000000000000000000000000000064",
             "f5b24dcea0e9381721a8c72784d30cfe64c11b4591226269f839d095b3e9cf10":"0000000000000000000000000000000000000000000000000000000000000064"}  // Key-value pairs in JSON Object
        }
    }
}
```

### Output JSON file: Return to SSVMRPC

#### Return result for executing a Rust program from SSVMRPC
```json
{
    "service_name": "ERC20",  // A string
    "uuid": "0x0000000012345678",  // 64 bits unsigned integer in hex string format
    "result":
    {
        "status": "Succeeded",  // Can be "Succeeded", "Failed", or "Reverted"
        "error_message": "...",  // String
        "gas": 123, // Gas given by Input JSON, in Integer format
        "gas_used": 100, // Used gas by this transaction, in Integer format
        "vm_snapshot": {
            "global" : [
                [0, "0x00000000FFFFFFFF"], [1, "0x00000000FFFFFFFF"]
                // List: [global_id(uint32), value_hex_string(64bit)]
            ],  // Global instance
            "memory" : [
                [0, "00000000"]
                // List: [memory_id(uint32), memory_dump_hex_string]
            ]   // Memory instance
        }, // Dumpped snapshot to restore VM, only in rust mode
        "return_value": ["-12345678"] // Return value list of function
    }
}
```

#### Return result for executing a Ethereum program from SSVMRPC
```json
{
    "service_name": "ERC20",  // A string
    "uuid": "0x0000000012345678",  // 64 bits unsigned integer in hex string format
    "result":
    {
        "status": "Succeeded",  // Can be "Succeeded", "Failed", or "Reverted"
        "error_message": "...",  // String
        "gas": 123, // Gas given by Input JSON, in Integer format
        "gas_used": 100, // Used gas by this transaction, in Integer format
        "storage": {"0000000000000000000000000000000000000000000000000000000000000000":"0000000000000000000000000000000000000000000000000000000000000064",
                        "f5b24dcea0e9381721a8c72784d30cfe64c11b4591226269f839d095b3e9cf10":"0000000000000000000000000000000000000000000000000000000000000064"},    // Key-value pairs in JSON Object
        "return_data": []  // JSON Array
    }
}
```

### Project Milestone
#### Phase 1

In the first phase, we will let SSVM execute a Rust program from the request of SSVMRPC.

1. Parsing SSVM's argument
2. Parsing Input JSON file for executing a Rust program
3. Execute Rust program
4. Compose output JSON

#### Phase 2

In the second phase, we will let SSVM execute a Ethereum program from the request of SSVMRPC.

1. Parsing Input JSON file for executing a Ethereum program
2. Encode argument in Ethereum format with given ABI

