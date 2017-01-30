{
    "targets": [
        {
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ],
            "target_name": "BridgeTest_1",
            "sources": ["code.cc","header.h"]
        }
    ]
}
