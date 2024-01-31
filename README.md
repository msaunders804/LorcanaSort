https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html#tutorial_windows_gitbash_build

# Function: Utilize computer vision to allow collectors to open packs of Lorcana cards in front of the camera and automaticcaly identify the matching card and store the card in the collectors "binder"

# 1/2: Update
1. Image input functional
2. Optical Character Recognition not working (Note Lorcana cards utilize light backgrounds with white letters so it is hard to distringuish letters)
3. Letters at the bottom of the card that notate set and number are too small for USB web camera quality (possibly useable for phone cameras?)

# 1/30: Update
1. Take image utilizing USB webcamera -> manipulate image to extract just the card
2. Generate hashes of all downloaded card images and store them in mySQL DB
3. Generate Perceptual hash of input image -> Compare input hash against database with Hamming Distance and find closest match
   
NOTE: PHASH is not a viable matching function as the image quality of input creates too large of a difference from downloaded image. Researching other options including Neural Network, Openzcv MstchTemplate and Feature Extraction; Research potental ORC with phone camera images (this will create the issue of turning the entire program into a phone app
