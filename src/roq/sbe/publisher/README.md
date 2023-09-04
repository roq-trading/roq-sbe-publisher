TODO

* cache encoded blob
* proper limited depth

firewall-cmd --permanent --direct --add-rule ipv4 filter INPUT 0 -m udp -p udp -m pkttype --pkt-type multicast -j ACCEPT
firewall-cmd --permanent --add-port=1234/udp
firewall-cmd --permanent --add-port=6789/udp

firewall-cmd --zone=public --add-rich-rule='rule family="ipv4" destination address="224.1.1.1" protocol value="ip" accept' --permanent
firewall-cmd --reload
