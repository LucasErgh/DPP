set(Dirs
    "../include"
)

set(Sources 
    "DPPClient.cpp"
)

add_library(src ${Sources})
target_include_directories(src PUBLIC ${Dirs})