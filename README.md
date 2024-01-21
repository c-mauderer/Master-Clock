# Master Clock

Note: Most of the documentation is written in English. But there is a short
German summary at the beginning.

## Summary (German)

Ziel des Projektes ist die Ansteuerung von Neben- beziehungsweise Tochteruhren ohne
zugehörige Mutteruhr. Als Energiequelle sollen Batterien verwendet werden.

Die Art von Nebenuhren, um die es hier geht, wurde z.B. häufig in Fabrikhallen
eingesetzt. Sehr häufig findet man sie z.B. mit Aufdruck von Siemens oder
ähnlichen Firmen. Die Nebenuhren sind so gebaut, dass der Minutenzeiger mit
einem Impuls um jeweils eine Minute weiterspringt. Dabei muss abwechselnd ein
positiver und ein negativer Spannungsimpuls angelegt werden. Üblicherweise
werden Uhren mit ±12 V oder ±24 V betrieben. Es scheint auch vereinzelte Modelle
mit höheren Spannungen zu geben.

Die Funktionsweise der Uhren wird von
[Rolf Süßbrich in einem ähnlichen Projekt](http://www.suessbrich.info/elek/ErsatzMutteruhr.html)
([archivierte Version](https://web.archive.org/web/20211020055925/http://www.suessbrich.info/elek/ErsatzMutteruhr.html))
gut erklärt. Die dort verwendete Schaltungsidee diente auch mir als Grundlage.
Allerdings habe ich zur Erzeugung des Minutenimpulses ein Mikrocontroller
eingesetzt, da dieser die Aufgabe stromsparender erledigen sollte, als
klassische CMOS-Bausteine. Statt am Eingang mit 10 Batterien für die 12V zu
arbeiten, wurde ein kleiner Step-Up-Regler aufgebaut, der auch für 24V Uhren
geeignet ist. Bei der Endstufe wurde die Idee aufgegriffen, einen
Operationsverstärker zur Ansteuerung zu verwenden.

### Useage (German)

Am Eingang der Platine kann eine Spannung von ca. 3 bis 10 V angelegt werden.
Höhere Spannungen führen dabei voraussichtlich zu niedrigeren Strömen beim
Schaltreglerteil. Der Prozessor wird über einen Linearregler versorgt und hat
daher einen konstante Stromaufnahme (und damit eine steigende Energieaufnahme
bei steigender Spannung). Der Schaltreglerteil sollte allerdings beim
Energiebudget überwiegen.

Am Ausgang wird der Motor der Uhr angeschlossen. Die Polarität ist dabei nicht
relevant.

Je nach Uhrwerk kann es sein, dass der Ausgangskondensator (C5) eventuell etwas
größer oder kleiner dimensioniert werden kann. Er sollte jedoch immer kleiner
als C4 sein. Kleinere Kondensatoren führen zu einer geringeren Stromaufnahme und
damit zu einer längeren Batterielebensdauer. Größere Kondensatoren liefern mehr
Energie für jeden Schritt des Zeigers.

Die Tasten werden einmal pro Sekunde ausgewertet, müssen also etwas länger
gedrückt werden. Die beiden Tasten haben folgende Funktionen:

PAUSE: Setzt die Minutensprünge aus. Während der Pausenzeit blinkt die LED jede
Sekunde kurz auf um den Zustand anzuzeigen. Kann mit STEP fortgesetzt
werden.

STEP: Wird die Taste mindestens eine Sekunde lang gedrückt, wird nach 5 Sekunden
ein Minutensprung ausgelöst. Ab diesem Zeitpunkt wird dann die nächste Minute
gezählt. Die 5 Sekunden sind notwendig, um dem Ausgangskondensator genügend Zeit
zum Laden zu geben. Ein erkannter Tastendruck wird mit einem kurzen Blinken der
LED bestätigt.

Beim Einlegen der Batterien oder manuellem Verstellen der Zeiger kann es
vorkommen, dass der erste Sprung ausbleibt. Das hängt von der Position des
Motors ab.

## Summary

The aim of the project is to control so called slave clocks without an
associated master clock. Batteries are to be used as the energy source.

The type of slave clocks in question were often used for example in factory
halls. They are very often found with the imprint of Siemens or similar
companies. The slave clocks are designed so that the minute hand advances by one
minute at a time with each electric pulse. A positive and a negative voltage
pulse must be applied alternately. Clocks are usually operated with ±12 V or ±24
V. There also appear to be a few models with higher voltages.

The operation of the clocks is well explained in German by
[Rolf Süßbrich in a similar project](http://www.suessbrich.info/elek/ErsatzMutteruhr.html)
([archived Version](https://web.archive.org/web/20211020055925/http://www.suessbrich.info/elek/ErsatzMutteruhr.html)).
I used the same basic idea as that project. However, I used a microcontroller to
generate the minute pulse, as this should do the job more energy-efficiently
than classic CMOS components. Instead of working with 10 batteries for the 12 V
input, a small step-up regulator was built, which is also suitable for 24 V
clocks. For the output stage, the idea of using an operational amplifier was
taken up.

### Useage Instructions

A voltage of approx. 3 to 10 V can be applied to the input of the circuit board.
Higher voltages are likely to lead to lower currents in the switching regulator
section. The processor is supplied via a linear regulator and therefore has a
constant current consumption (and thus an increasing energy consumption with
increasing voltage). However, the switching regulator part should predominate in
the energy budget.

The motor of the clock is connected to the output. The polarity is not relevant.

Depending on the movement, the output capacitor (C5) may be slightly larger or
smaller. However, it should always be smaller than C4. Smaller capacitors result
in lower current consumption and therefore a longer battery life. Larger
capacitors supply more energy for each step of the minute hand.

The buttons are analysed once per second, so they must be pressed a little
longer than that. The two buttons have the following functions:

PAUSE: Pauses the minute jumps. During the pause time, the LED flashes briefly
every second to indicate the status. Can be continued with STEP.

STEP: If the button is pressed for at least one second, a minute jump is
triggered after 5 seconds. The next minute is then counted from this point. The
5 seconds are necessary to give the output capacitor enough time to charge. A
recognised button press is confirmed by the LED flashing briefly.

When inserting the batteries or manually adjusting the hands, the first jump may
not occur. This depends on the initial position of the motor.
