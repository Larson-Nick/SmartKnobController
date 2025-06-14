Im creating this folder because of a really annoying issue I encountered with this workflow.

Basically the easyeda2kicad script generates the symbol text file in a specific spacing that KiCAD accepts but doesn't like.
When you first use and save the library or any symbols in KiCAD, it automatically reformats it.
Now, when you use the script to add another symbol to an existing library, there are mixed formats and KiCAD doesn't recognize your symbol (footprint and 3D model are fine). When you save a library with "mixed formatting," KiCAD just pitches the stuff it doesn't recognize.

So....

I made this BandAid folder as a placeholder to generate just a symbol, save it in KiCAD so it gets reformatted, and then it can be pasted into the desired symbol library folder.

DELETE THE SYMBOL LIBRARY AFTER IT'S USED.

How To:

- Generate the full part as normal in the desired library

- Save the library in the symbol editor

- Use this line:
	easyeda2kicad --symbol --lcsc_id=C... --output "~\LIBRARIES\BANDAID" (CHANGE THE SQUIGGLE)

- Save the BANDAID library

- Extract the properly formatted symbol from the bottom of the BANDAID text file.

- Paste it into the desired library text file (IMPORTANT: Paste before the last closed parenthesis)

- DELETE THE BANDAID LIBRARY