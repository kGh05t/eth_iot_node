// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;

contract SensorDataStorage {
    mapping(string => string) private dataHashes;

    function uploadData(string memory timestamp, string memory hash) public {
        dataHashes[timestamp] = hash;
    }

    function getHash(string memory timestamp) public view returns (string memory) {
        return dataHashes[timestamp];
    }
}
