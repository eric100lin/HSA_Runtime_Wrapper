HSA_Runtime_Wrapper
===============================

The HSA_Runtime_Wrapper is a library to let the HSA programming easily. It wrap the HSA runtime API with some high-level C++ class.

o Steps to compile the library and run the sample:

     1. cd ~
     2. git clone https://github.com/eric100lin/HSA_Runtime_Wrapper.git
     3. cd HSA_Runtime_Wrapper
     4. make
     5. export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/HSA_Runtime_Wrapper/library
     6. cd application
     7. ./HSA_Runtime_Wrapper_VectorCopy


  o Written by : NTHU PLlab Tzu-Hsiang Lin, 
    National Tsing Hua University Programming Language Laboratory
    
  o If you want to participate in actively developing HSA_Runtime_Wrapper
       please send an email to `thlin@pllab.cs.nthu.edu.tw'.