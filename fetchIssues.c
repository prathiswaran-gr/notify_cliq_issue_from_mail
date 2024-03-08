
channelUniqueName = "testingl";
counter = 1;
portalResponse = invokeurl
[
	url :"https://projectsapi.zoho.in/restapi/portals/"
	type :GET
	connection:"oauth"
];
portalId = portalResponse.get("portals").get(0).get("id");
// default portal
projectId = 217431000000035009;
allBugsResponse = invokeurl
[
	url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/"
	type :GET
	connection:"oauth"
];
event = "";
eventLink = "";
process = "";
processName = "";
rowsList = list();
for each  bug in allBugsResponse.get("bugs")
{
	id = bug.get("id");
	event = bug.get("title");
	status = bug.get("status").get("type");
	if(status.equals("Open"))
	{
		commentResponse = invokeurl
		[
			url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
			type :GET
			connection:"oauth"
		];
		unFormattedCommentData = commentResponse.get("comments").get(0).get("comment");
		commentFormattedList = unFormattedCommentData.toList(",");
		for each  data in commentFormattedList
		{
			dataList = data.toList("=");
			key = dataList.get(0);
			if(dataList.size() != 1)
			{
				value = dataList.get(1);
			}
			if(key == "eventLink")
			{
				decodedLink = htmlDecode(dataList.get(1) + "=" + dataList.get(2) + "=" + dataList.get(3) + "=" + dataList.get(4) + "=" + dataList.get(5));
				linkStartIndex = decodedLink.find(">") + 1;
				eventLink = decodedLink.substring(linkStartIndex,decodedLink.length() - 4);
			}
			if(key == "process")
			{
				process = value;
			}
			else if(key == "processName")
			{
				processName = value;
			}
			rowList0 = Map();
			rowList0.put("*S.no*",counter);
			rowList0.put("Event","[" + event + "](" + eventLink + ")");
			if(process != "")
			{
				rowList0.put("Process / name",process);
			}
			else
			{
				rowList0.put("Process / name",processName);
			}
			rowList0.put("Bug info","[Click here](" + bug.get("link").get("web").get("url") + ")");
		}
		rowsList.add(rowList0);
		counter = counter + 1;
	}
}
response = Map();
response.put("text","Pending issues were listed below !");
bot = Map();
bot.put("name","Scheduler");
bot.put("image","https://theworkflowacademy.com/wp-content/uploads/2022/05/zoho-Projects.png");
response.put("bot",bot);
card = Map();
card.put("title","Reminder 📍");
card.put("icon","https://theworkflowacademy.com/wp-content/uploads/2022/05/zoho-Projects.png");
response.put("card",card);
slidesList = list();
slidesList0 = Map();
slidesList0.put("type","table");
slidesList0.put("title","Bug details 🐞");
data = Map();
headersList = list();
headersList.add("*S.no*");
headersList.add("Event");
headersList.add("Process / name");
headersList.add("Bug info");
data.put("headers",headersList);
data.put("rows",rowsList);
slidesList0.put("data",data);
slidesList.add(slidesList0);
response.put("slides",slidesList);
if(counter != 0)
{
	zoho.cliq.postToChannel(channelUniqueName,response);
}
