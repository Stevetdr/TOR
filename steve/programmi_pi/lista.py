

li=['a', 'b', 'mpilgrim', 'z', 'example', 'new', 'newone', 'newagain', 'foxtrot', 'cossa', 'crozza', 'tortora', 'pistacchio', 'piccione', 'palla', 'bianca', 'bianca1', 'bianca2', 'bianca3', 'caneddu', 'cannes', 'rasega']

li.append("raspa")
li.append("pasga")
li.append("fritto")
li.append("f1ritto")
li.append("fr2itto")
li.append("fri3tto")
li.append("frit4to")
li.append("fritt5o")
li.append("fritto6")
li.append("fritto7")
li.append("fritto8")
li.append("otto8")

init = 'bia'
into = 'tto'
#for i in range(len(li)):
#    print (li[i])
print("---------------- inizia con ", init)
for i in range(len(li)):
    #print (li[i][:3])
    if(li[i][:3] == init):
        print (li[i][:3],li[i])
print("----------------------------------")
print("******************contiene :", into)
for i in range(len(li)):
    #print (li[i][:3])
    if(into in li[i]):
        print (li[i],li[i])
print("**********************************")
#print (li)

    
