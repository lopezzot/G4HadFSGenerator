from ROOT import *


def OverlapHisto(pl, energy, material):
    file1043 = TFile(pl+"proton"+energy+material+"_1043.root")
    file1051 = TFile(pl+"proton"+energy+material+"_1051.root")
    file1063 = TFile(pl+"proton"+energy+material+"_1063.root")
    file1071 = TFile(pl+"proton"+energy+material+"_1071.root")
    file1103 = TFile(pl+"proton"+energy+material+"_1103.root")

    h1eloss1043 = file1043.Get("E_loss")
    h1eloss1051 = file1051.Get("E_loss")
    h1eloss1063 = file1063.Get("E_loss")
    h1eloss1071 = file1071.Get("E_loss")
    h1eloss1103 = file1103.Get("E_loss")

    celoss = TCanvas("e_loss", "e_loss", 500, 500)
    legende_loss = TLegend(0.1, 0.7, 0.48, 0.9)
    h1eloss1103.SetAxisRange(0., 3800., "Y")
    h1eloss1103.Draw("histo")
    legende_loss.AddEntry(h1eloss1103, "G411.0.p03-"+pl, "lep")
    h1eloss1071.SetMarkerColor(kRed)
    h1eloss1071.SetLineColor(kRed)
    h1eloss1071.Draw("same histo")
    legende_loss.AddEntry(h1eloss1071, "G410.7.p01-"+pl, "lep")
    h1eloss1063.SetMarkerColor(kBlue)
    h1eloss1063.SetLineColor(kBlue)
    h1eloss1063.Draw("same histo")
    legende_loss.AddEntry(h1eloss1063, "G410.6.p03-"+pl, "lep")
    h1eloss1051.SetMarkerColor(kGreen)
    h1eloss1051.SetLineColor(kGreen)
    h1eloss1051.Draw("same histo")
    legende_loss.AddEntry(h1eloss1051, "G410.5.p01-"+pl, "lep")
    h1eloss1043.SetMarkerColor(6)
    h1eloss1043.SetLineColor(6)
    h1eloss1043.Draw("same histo")
    legende_loss.AddEntry(h1eloss1043, "G410.4.p03-"+pl, "lep")
    legende_loss.Draw()

    h1mom1043 = file1043.Get("Momentum_conservation")
    h1mom1051 = file1051.Get("Momentum_conservation")
    h1mom1063 = file1063.Get("Momentum_conservation")
    h1mom1071 = file1071.Get("Momentum_conservation")
    h1mom1103 = file1103.Get("Momentum_conservation")

    cmom = TCanvas("mom", "mom", 500, 500)
    legende_mom = TLegend(0.1, 0.7, 0.48, 0.9)
    h1mom1043.SetMarkerColor(6)
    h1mom1043.SetLineColor(6)
    h1mom1043.SetAxisRange(0., 18000., "Y")
    h1mom1043.Draw("histo")
    legende_mom.AddEntry(h1mom1043, "G410.4.p03-"+pl, "lep")
    h1mom1051.SetMarkerColor(kGreen)
    h1mom1051.SetLineColor(kGreen)
    h1mom1051.Draw("same histo")
    legende_mom.AddEntry(h1mom1051, "G410.5.p01-"+pl, "lep")
    h1mom1063.SetMarkerColor(kBlue)
    h1mom1063.SetLineColor(kBlue)
    h1mom1063.Draw("same histo")
    legende_mom.AddEntry(h1mom1063, "G410.6.p03-"+pl, "lep")
    h1mom1071.SetMarkerColor(kRed)
    h1mom1071.SetLineColor(kRed)
    h1mom1071.Draw("same histo")
    legende_mom.AddEntry(h1mom1071, "G410.7.p01-"+pl, "lep")
    h1mom1103.Draw("same histo")
    legende_mom.AddEntry(h1mom1103, "G411.0.p03-"+pl, "lep")
    legende_mom.Draw()

    h1pi01043 = file1043.Get("Pi0_energy")
    h1pi01051 = file1051.Get("Pi0_energy")
    h1pi01063 = file1063.Get("Pi0_energy")
    h1pi01071 = file1071.Get("Pi0_energy")
    h1pi01103 = file1103.Get("Pi0_energy")

    cpi0 = TCanvas("pi0", "pi0", 500, 500)
    legende_pi0 = TLegend(0.1, 0.7, 0.48, 0.9)
    h1pi01103.Draw("histo")
    legende_pi0.AddEntry(h1pi01103, "G411.0.p03-"+pl, "lep")
    h1pi01071.SetMarkerColor(kRed)
    h1pi01071.SetLineColor(kRed)
    h1pi01071.Draw("same histo")
    legende_pi0.AddEntry(h1pi01071, "G410.7.p01-"+pl, "lep")
    h1pi01063.SetMarkerColor(kBlue)
    h1pi01063.SetLineColor(kBlue)
    h1pi01063.Draw("same histo")
    legende_pi0.AddEntry(h1pi01063, "G410.6.p03-"+pl, "lep")
    h1pi01051.SetMarkerColor(kGreen)
    h1pi01051.SetLineColor(kGreen)
    h1pi01051.Draw("same histo")
    legende_pi0.AddEntry(h1pi01051, "G410.5.p01-"+pl, "lep")
    h1pi01043.SetMarkerColor(6)
    h1pi01043.SetLineColor(6)
    h1pi01043.Draw("same histo")
    legende_pi0.AddEntry(h1pi01043, "G410.4.p03-"+pl, "lep")
    legende_pi0.Draw()

    h1n1043 = file1043.Get("Neutron_kenergy")
    h1n1051 = file1051.Get("Neutron_kenergy")
    h1n1063 = file1063.Get("Neutron_kenergy")
    h1n1071 = file1071.Get("Neutron_kenergy")
    h1n1103 = file1103.Get("Neutron_kenergy")

    cn = TCanvas("neutron_kenergy", "neutron_kenergy", 500, 500)
    legende_n = TLegend(0.1, 0.7, 0.48, 0.9)
    h1n1103.Draw("histo")
    legende_n.AddEntry(h1n1103, "G411.0.p03-"+pl, "lep")
    h1n1071.SetMarkerColor(kRed)
    h1n1071.SetLineColor(kRed)
    h1n1071.Draw("same histo")
    legende_n.AddEntry(h1n1071, "G410.7.p01-"+pl, "lep")
    h1n1063.SetMarkerColor(kBlue)
    h1n1063.SetLineColor(kBlue)
    h1n1063.Draw("same histo")
    legende_n.AddEntry(h1n1063, "G410.6.p03-"+pl, "lep")
    h1n1051.SetMarkerColor(kGreen)
    h1n1051.SetLineColor(kGreen)
    h1n1051.Draw("same histo")
    legende_n.AddEntry(h1n1051, "G410.5.p01-"+pl, "lep")
    h1n1043.SetMarkerColor(6)
    h1n1043.SetLineColor(6)
    h1n1043.Draw("same histo")
    legende_n.AddEntry(h1n1043, "G410.4.p03-"+pl, "lep")
    legende_n.Draw()

    outputfile = TFile("Output"+pl+".root", "RECREATE")
    celoss.Write()
    cmom.Write()
    cpi0.Write()
    cn.Write()
    outputfile.Close()


#OverlapHisto("QGSP", "100.", "G4_N")
#OverlapHisto("FTFP", "100.", "G4_N")
#OverlapHisto("FTFP", "10.0", "G4_Cu")
OverlapHisto("BERT", "2.00", "G4_Cu")
