// i 1 3 0 4 2
// o 0 1 3 4
OPENQASM 2.0;
include "qelib1.inc";
qreg q[5];
creg c[4];
u3(1.5707963267948968,1.5707963267948966,1.6448413411355385) q[3];
u2(1.570796326794897,4.638343966044049) q[4];
cx q[3],q[4];
u3(1.570796326794897,3.141592653589793,1.5707963267948972) q[3];
u3(1.5707963267948961,4.71238898038469,0.0) q[4];
cx q[3],q[4];
u3(0.07404501434064163,-2.3561944901923435,-1.5707963267948972) q[3];
cx q[1],q[3];
u1(0.7853981633974483) q[3];
u3(3.1415926535897927,4.152907526272001,-0.48543643977204676) q[4];
cx q[4],q[3];
u1(-0.7853981633974483) q[3];
cx q[1],q[3];
u1(0.7853981633974483) q[4];
cx q[3],q[4];
cx q[4],q[3];
cx q[3],q[4];
cx q[1],q[3];
u1(0.7853981633974483) q[1];
u1(-0.7853981633974483) q[3];
cx q[1],q[3];
cx q[0],q[1];
cx q[1],q[0];
cx q[0],q[1];
cx q[1],q[3];
cx q[3],q[1];
cx q[1],q[3];
cx q[0],q[1];
u3(1.5707963267948968,-1.5707963267948966,1.5707963267948966) q[3];
u3(1.5707963267948957,-3.141592653589793,8.639379797371932) q[4];
cx q[3],q[4];
u3(1.570796326794897,3.141592653589793,1.5707963267948972) q[3];
u3(1.5707963267948961,4.71238898038469,0.0) q[4];
cx q[3],q[4];
u3(6.537266308273344e-16,4.828816143926751,0.6689709998553872) q[3];
cx q[1],q[3];
u1(0.7853981633974483) q[3];
u3(3.7096710452298594e-16,1.290649408685156,3.4217395716995336) q[4];
cx q[4],q[3];
u1(-0.7853981633974483) q[3];
cx q[1],q[3];
u3(3.141592653589793,2.61432379533507e-16,1.3493779808430516e-16) q[1];
u1(0.7853981633974483) q[4];
cx q[3],q[4];
cx q[4],q[3];
cx q[3],q[4];
u3(1.0207367487686442e-15,0.018604670131958434,-3.1601973237217518) q[3];
cx q[1],q[3];
u3(3.1415926535897927,-1.9289902371411531,0.4272042530511918) q[1];
u3(3.1415926535897927,-2.427143017494427,-1.6417448540969786) q[3];
u2(0.0,3.9269908169872414) q[4];