from cmio import MaterialList

if __name__=="__main__":
    import cmio
    material0 = cmio.MqoMaterial(cmio.Rgba(1,0,0,0),
                                 1.0,
                                 1.0,
                                 1.0,
                                 1.0,
                                 10.0)
    material_list = cmio.MaterialList()
    material_list.append_material(material0)
    
    mat = material_list.get_material(0)
    print mat
    print "%x" % mat.this