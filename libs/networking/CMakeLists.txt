add_library(networking 
            Client_Interface.hpp 
            Connection.hpp 
            net_message.hpp 
            Server_Interface.hpp 
            tsqueue.hpp)
            
set_target_properties(networking PROPERTIES LINKER_LANGUAGE CXX)

