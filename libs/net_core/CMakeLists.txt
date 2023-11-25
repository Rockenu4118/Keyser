add_library(net_core 
            Client_Interface.hpp 
            Connection.hpp 
            net_message.hpp 
            Server_Interface.hpp 
            tsqueue.hpp)
            
set_target_properties(net_core PROPERTIES LINKER_LANGUAGE CXX)

