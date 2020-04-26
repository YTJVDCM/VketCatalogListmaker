require 'json'
require 'time'
require 'open3'

ENV['INLINEDIR'] = File.dirname(File.expand_path(__FILE__))

o,s=Open3.capture2("powershell -Command \"[Environment]::GetFolderPath('MyDocuments')\"")
o = o.chomp
p o
t=Time.now
File.open(o+"\\VketJSON\\cataloglist.json","r") do |f|
    data=f.read
    jsondata=JSON.parse(data)
    p jsondata["VketData"].length
    p jsondata["VketData"][0]
    filetimedata=o+"\\VketJSON\\Output-"+t.year.to_s+"-"+t.month.to_s+"-"+t.day.to_s+"-"+t.hour.to_s+"-"+t.min.to_s+"-"+t.sec.to_s+".html"
    File.open(filetimedata,"w") do |fw|
        fw.puts("<!DOCTYPE html>")
        fw.puts("<html lang=\"ja\">")
        fw.puts("<head>")
        fw.puts("<meta charset=\"UTF-8\">")
        fw.puts("<title>VketJSON output data</title>")
        fw.puts("</head>")
        fw.puts("<body style=\"background-color:#F9F9F9;\">")
        fw.puts("<ul style=\"list-style: none;text-shadow:0px 0px 10px black;color:white;font-size:120%;font-weight:bold;\">")
        for i in 0..jsondata["VketData"].length-1 do
            fw.puts("<div style=\"background-color:white;background-image:url(\'"+jsondata["VketData"][i]["Header"].chomp+"\');background-size: cover;padding:20px;margin:1%;display:inline-block;width:40%\">")
            fw.puts("<li><a href=\""+jsondata["VketData"][i]["URL"]+"\"><img src=\""+jsondata["VketData"][i]["Icon"]+"\" width=128px height=128px style=\"border-radius:50%;vertical-align:middle;margin:20px;\"><h2 style=\"color:white;font-size:200%;display:inline;margin:20px;\">"+jsondata["VketData"][i]["Title"]+"</h2></a></li>")
            fw.puts("<span style=\"margin:10px;\">World : "+jsondata["VketData"][i]["World"]+"</span><br>")
            fw.puts("<span style=\"margin:10px;\">Group : "+jsondata["VketData"][i]["Group"]+"</span><br>") if jsondata["VketData"][i]["Group"] != "Missing"
            fw.puts("<span style=\"margin:10px;\">Section : "+jsondata["VketData"][i]["Section"]+"</span><br>") if jsondata["VketData"][i]["Section"] != "Missing"
            fw.puts("<span style=\"margin:10px;\">Vket : "+jsondata["VketData"][i]["Vket"]+"</span><br>") if jsondata["VketData"][i]["Vket"] != "Missing"
            fw.puts("</div>")
        end
        fw.puts("</ul>")
        fw.puts("</body>")
        fw.puts("</html>")
    end
end
rename="REN \""+o+"\\VketJSON\\cataloglist.json\" cataloglist-"+t.year.to_s+"-"+t.month.to_s+"-"+t.day.to_s+"-"+t.hour.to_s+"-"+t.min.to_s+"-"+t.sec.to_s+".json\""
p rename
Open3.capture2(rename)