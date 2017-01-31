{
    "targets": [
        {
            "link_settings": {
                "libraries": ["../lib/libusb-1.0.lib"]
            },
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "include"
            ],
            "target_name": "BridgeTest_2",
            "sources": ["GCAdapter.cc","GCAdapter.h"]
        }
    ]
}
