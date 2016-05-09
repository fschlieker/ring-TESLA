# Optimized ring-TESLA

This is a mirror of ring-TESLA, a lattice-based digital signature scheme presented in [1] by Akleylek, Bindel, Buchmann, Krämer and Marson (as the initial commit) along with some performance optimizations (in the subsequent commits). 

The paper [1] is available on [IACR ePrint](https://eprint.iacr.org/2016/030). More information can be found [here](https://www.cdc.informatik.tu-darmstadt.de/en/cdc/staff/nina-bindel/) and the software can be downloaded from [here](https://www.cdc.informatik.tu-darmstadt.de/fileadmin/user_upload/Group_CDC/ring-TESLACode.zip).

The goal is to demonstrate possible optimizations by rewriting parts of the code and leveraging modern processor architecture features. The following table presents our performance improvements, measured with the included testbench on an Intel(R) Core(TM) i7-4600U CPU @ 2.70 GHz.

| Operation   | Sign cycles (crypto_sign) | Improvement | Verify cycles (crypto_sign_open) | Improvement |
| ------------| -------------------------:| -----------:|---------------------------------:| -----------:|
| Baseline [1]|                    522404 |             |                       209716     |             |
| Optimized   |                    199515 |       2.62x |                        94088     |       2.23x |

Authors:
--------

* Shay Gueron (1, 2)
* Fabian Schlieker (3)

(1) Intel Corporation, Israel Development Center, Haifa, Israel  
(2) University of Haifa, Israel  
(3) Ruhr University Bochum, Germany

Copyright:
----------

Modified work Copyright (c) 2016, Shay Gueron and Fabian Schlieker

Reference:
----------
[1] S. Akleylek, N. Bindel, J. Buchmann, J. Krämer, G. A. Marson, _"An Efficient Lattice-Based Signature Scheme with Provably Secure Instantiation"_, AFRICACRYPT 2016