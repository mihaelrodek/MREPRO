# terminiraj sve postojece programi koji mozda vec rade
himage CnC killall -9 CandC.py
himage BOT1 killall -9 bot
himage UDP_server killall -9 UDP_server

# kopiraj sve potrebne datoteke na cvorove
hcp CandC.py CnC:/
hcp bot BOT1:/
hcp UDP_server UDP_server:/

# postavi IP adresu i port UPD servera u CandC.py programu na dobre vrijednosti
himage CnC sed -i .bak 's/UDP_ip = "\([^"]*\)"/UDP_ip = "10.0.0.20"/' /CandC.py
himage CnC sed -i .bak 's/UDP_port = "\([^"]*\)"/UDP_port = "1234"/' /CandC.py

# pokreni sve programe u posebnim terminal prozorima
xfce4-terminal -T 'CandC.py' -e "bash -c 'himage CnC python2.7 /CandC.py';bash"
xfce4-terminal -T 'UDP_server' -e "bash -c 'himage UDP_server /UDP_server -p \"ovo je payload\"';bash"
xfce4-terminal -T 'zrtva1' -e "bash -c 'himage zrtva1 nc -kul 1111';bash"
xfce4-terminal -T 'zrtva2' -e "bash -c 'himage zrtva2 nc -kul 2222';bash"
xfce4-terminal -T 'zrtva3' -e "bash -c 'himage zrtva3 nc -kul 3333';bash"
sleep 2
xfce4-terminal -T 'bot' -e "bash -c 'himage BOT1 /bot 10.0.0.10 5555';bash"
