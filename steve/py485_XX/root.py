

py485_XX
	|
	+--- __main__.py
	|
	+--- Source
	|		|
	|		+--- __init__.py
	|		|
	|		+--- Main
	|		|	    |
	|		|	    +--- OpenRs485Port.py
	|		|
	|		+--- Process
	|				|
	|				+--- DigitalPinSS.py
	|				|
	|				+--- SendToRealay.py
	+--- bin
	      |
		  +--- LnLib_2017-12-11.zip
		  			|
		  			LnLib (nome della directory zippata)
		  			|
		  			+--- __init__.py  (importante!) vedi sotto
		  			+--- Common
		  			+--- Dict
		  			+--- File
		  			+--- LnRS485
		  			+--- Monkey
		  			+--- ParseInput
		  			+--- Process
		  			+--- String
		  			+--- System



# -----------------------------------------------------------

Nella __init__.py sotto LnLib posso trovare:

# ---------- LnLIB DotMap dictionary ------
from . Dict.LnDict_DotMap              import DotMap  as Dict

# ---------- RS485 functions ------
from . LnRS485.LnRs485_Class           import LnRs485 as Rs485 # import di un membro
