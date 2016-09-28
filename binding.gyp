{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "addon.cc", "annoyindexwrapper.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
