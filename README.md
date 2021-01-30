# Emulator

This is an emulator written in C++, for emulating games packed by the [assembler](https://github.com/rusudinu/assembler), that packs the game written in assembly into a .rom file that is then parsed by this emulator. You can find the assembler [here](https://github.com/rusudinu/assembler).

This was written for a hackathon I attended in 2020. You can find the request in this readme, but it's written in romanian.


# Hackathon request

Proba Open - Utilitar 2020

Scopul acestei probe este dezvoltarea unui emulator pentru o platformă hardware imaginară dedicată jocurilor video, denumit și “fantasy game console”.

Pentru aceasta probă va trebui să dezvoltați urmatoarele lucruri:

Un set de instrucțiuni pentru o platformă imaginară cu anumite limitari. [10p] Un assembler pentru set-ul de instrucțiuni. [10p] Un emulator pentru set-ul de instrucțiuni. [20p] Un joc exemplu în acest set de instrucțiuni care funcționează pe emulator. [20p]

Va trebui să incercați să respectați următoarele limitări [20p]:

RAM: 128 octeți. ROM: 4000 octeți. Video RAM: 3840 octeti. Rezoluție: 40 x 192 pixeli. 6 regiștri generali. 4 regiștri speciali pentru contorul programului, indicatorul de stiva, date de intrare, și o utilitate la alegerea voastră. 6507 de instrucțiuni per cadru, 60 cadre pe secundă. Toți regiștrii au o dimensiune de un octet. Nu sunt permise decat operatiile cu numere intregi.

Recomandăm ca emulatorul să ofere și un ecran de statistici de sistem pentru a putea verifica respectarea limitărilor.

Pentru creativitate și respectarea spiritului probei se vor acorda 20 de puncte.