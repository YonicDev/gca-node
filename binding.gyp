{
    "targets": [
        {
            "conditions": [
                ["OS=='win'",{
                    "link_settings": {
                        "libraries":["../lib/libusb-1.0.lib"]
                    }
                }],
                ["OS=='linux'", {
                    "libraries": ["<!@(pkg-config --libs --cflags libusb-1.0)"]
                }]
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "include"
            ],
            "target_name": "gca-node",
            "sources": ["GCAdapter.cc","GCAdapter.h"]
        }
    ]
}
