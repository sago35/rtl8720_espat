# Arduino sketch for RTL8720DN espat

## How to use

1) Update the Wireless Core Firmware  
https://wiki.seeedstudio.com/Wio-Terminal-Network-Overview/  

2) Upload a Sketch or copy `*.uf2` to MSD  
You can download uf2 (for wio terminal only) from the release page.  

## Links

AT command list
https://github.com/Seeed-Studio/seeed-ambd-sdk

## Examples

```
Begin SPI:

Connecting
Ready! Enter some AT commands
[0D[0A
ready[0D[0A
```


```
AT
OK[0D[0A
```

```
AT+CWLAP
+CWLAP:(3,"DIxxxxxxxxxxxxxxxxx",-37,"xx:yy:zz:xx:yy:zz",11)[0D[0A
+CWLAP:(4,"sgxxxxxxx",-63,"xx:yy:zz:xx:yy:zz",8)[0D[0A
+CWLAP:(4,"",-65,"xx:yy:zz:xx:yy:zz",36)[0D[0A
+CWLAP:(4,"",-66,"xx:yy:zz:xx:yy:zz",8)[0D[0A
+CWLAP:(3,"elxxxxxxxxxf3",-68,"xx:yy:zz:xx:yy:zz",100)[0D[0A
+CWLAP:(3,"elxxxxxxxxxf3",-71,"xx:yy:zz:xx:yy:zz",10)[0D[0A
+CWLAP:(3,"DIxxxxxxxxxxxxxxxxxxxxxxxsTD",-72,"xx:yy:zz:xx:yy:zz",36)[0D[0A
+CWLAP:(1,"auxxxxxxxxxxxxx",-94,"xx:yy:zz:xx:yy:zz",5)[0D[0A
+CWLAP:(3,"Buxxxxxxxxxxxx",-95,"xx:yy:zz:xx:yy:zz",2)[0D[0A
+CWLAP:(3,"auxxxxxxxxxxx",-95,"xx:yy:zz:xx:yy:zz",5)[0D[0A
+CWLAP:(3,"prxxxxxxxxxxxxx",-109,"xx:yy:zz:xx:yy:zz",11)[0D[0A
OK[0D[0A
```
