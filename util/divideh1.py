'''
pyROOT script to divide Pz weighted by Pt histo by Pz histo.
The result is distribution of Pt against Pz.
Usage: python3 divideh1.py
'''
import ROOT as root
file = root.TFile("FTFPpi-10.0G4_Cu.root")
h1 = file.Get("Pi-_Pz")
h2 = file.Get("Pi-_Pz_wPt")
h2.Divide(h1)
h2.Draw()
