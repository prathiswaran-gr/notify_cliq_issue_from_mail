messageDetails = zoho.mail.getMessage(mail_messageId,"projectxmail");
text = htmlEncode(messageDetails.get("CONTENT"));

portalResponse = invokeurl
[
	url :"https://projectsapi.zoho.in/restapi/portals/"
	type :GET
	connection:"projectxmail"
];
portalId = portalResponse.get("portals").get(0).get("id"); // default portal
projectId = "";
MYSQL_MASTER_SLOW_QUERIES = text.getOccurenceCount("MYSQL_MASTER_SLOW_QUERIES&");
MYSQL_HIGH_USER_CONNECTIONS = text.getOccurenceCount("MYSQL_HIGH_USER_CONNECTIONS&");
PROCESS_DOWN = text.getOccurenceCount("PROCESS_DOWN&");
// Tags
SERVER_PERFORMACE_TAG_ID = "";



if(MYSQL_HIGH_USER_CONNECTIONS > 0){
	counter = leftpad("1",MYSQL_HIGH_USER_CONNECTIONS).replaceAll(" ","1,").toList();
	newText = text;
	for each iterator in counter {
		ip = "";
		event = "";
		eventLink = "";
		process = "";
		processName = "";
		domain = "";
		eventExtension = "";
		startIndex = text.find("MYSQL_HIGH_USER_CONNECTIONS&");
		newStartIndex = startIndex - 2200;
		endIndex = startIndex + 1540;
		newText = text.substring(newStartIndex, endIndex);

		if(newStartIndex < 0){
			newStartIndex = 0;
		}
		if(endIndex > text.length()){
			endIndex = text.length();
		}

		if(newText.contains("us4-mi.zohodcm")){
			domain = "us4-mi.zohodcm.com";
			eventExtension = "US4";
		}
		else if(newText.contains("in2-mi.zohodcm")){
			domain = "in2-mi.zohodcm.com";
			eventExtension = "IN2";
		}
		else if(newText.contains("ca1-mi.zohodcm")){
			domain = "ca1-mi.zohodcm.com";
			eventExtension = "CA1";
		}
		else if(newText.contains("cn2-mi.zohodcm")){
			domain = "cn2-mi.zohodcm.com";
			eventExtension = "CN2";
		}
		else if(newText.contains("eu1-mi.zohodcm")){
			domain = "eu1-mi.zohodcm.com";
			eventExtension = "EU1";
		}
		else if(newText.contains("ca1-mi.zohodcm")){
			domain = "ca1-mi.zohodcm.com";
			eventExtension = "CA1";
		}
		else if(newText.contains("jp1-mi.zohodcm")){
			domain = "jp1-mi.zohodcm.com";
			eventExtension = "JP1";
		}
		else if(newText.contains("in2nic-mi.zohodcm")){
			domain = "in2nic-mi.zohodcm.com";
			eventExtension = "IN2NIC";
		}
		else if(newText.contains("sa1-mi.zohodcm")){
			domain = "sa1-mi.zohodcm.com";
			eventExtension = "SA1";
		}
		else if(newText.contains("uk1-mi.zohodcm")){
			domain = "uk1-mi.zohodcm.com";
			eventExtension = "UK1";
		}
		else if(newText.contains("au1-mi.zohodcm")){
			domain = "au1-mi.zohodcm.com";
			eventExtension = "AU1";
		}
		else if(newText.contains("us4charm-mi.zohodcm")){
			domain = "us4charm-mi.zohodcm.com";
			eventExtension = "US4Charm";
		}

		if(newText.contains("time="))
		{
			startIndex = newText.find("time=");
			endIndex = startIndex + 200;
			list = newText.substring(startIndex,endIndex).toList("");
			url = "";
			for each  ch in list
			{
				if(ch == "&")
				{
					break;
				}
				url = url + ch;
			}
			modifiedText = newText.replaceAll(url,"");
			newText = modifiedText;
			eventLink = "<a href='https://"+domain+"/serviceMap.do?serviceName=Chat&'"+url+">https://"+domain+"/serviceMap.do?serviceName=Chat&"+url+"</a>";
			
		}
		
		if(newText.contains("IP&")){
			ipStartIndex = newText.find("IP&") + 39;
			ipEndIndex = ipStartIndex + 14;
			unFormattedIpList = newText.subString(ipStartIndex,ipEndIndex).toList("");
			for each  ch in unFormattedIpList
			{
				if(ch == '&')	
				{
					break;
				}
				ip = ip + ch;
			}
		}
	
	

		if(newText.contains("Event&"))
		{
			eventStartIndex = newText.find("Event&") + 270;
			eventEndIndex = eventStartIndex + 30;
			// appx max characters length
			unFormattedEventList = newText.subString(eventStartIndex,eventEndIndex).toList("");
			for each  ch in unFormattedEventList
			{
				if(ch == '&')
				{
					break;
				}
				event = event + ch;
			}
			event+="_"+eventExtension;
		}
		
		if(newText.contains("Process&") && (newText.find("Process&")))
		{
			processStartIndex = newText.find("Process&") + 109;
			processEndIndex = processStartIndex + 40;
			// appx max characters length
			unFormattedProcessList = newText.subString(processStartIndex,processEndIndex).toList("");
			for each  ch in unFormattedProcessList
			{
				if(ch == '&')
				{
					break;
				}
				process = process + ch;
			}
		}
		else if(newText.contains("Process Name&"))
		{
			processNameStartIndex = newText.find("Process Name&") + 114;
			processNameEndIndex = processNameStartIndex + 30;
			// appx max characters length
			unFormattedProcessNameList = newText.subString(processNameStartIndex,processNameEndIndex).toList("");
			for each  ch in unFormattedProcessNameList
			{
				if(ch == '&')
				{
					break;
				}
				processName = processName + ch;
			}
		}

		text = text.replaceFirst("MYSQL_HIGH_USER_CONNECTIONS&","MYSQL__HIGH__USER__CONNECTIONS&");

		
		allBugsResponse = invokeurl
		[
			url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/"
			type :GET
			connection:"projectxmail"
		];

		numberOfOpenIssues = 0;
		for each bug in allBugsResponse.get("bugs") {
			isClosed = bug.get("closed");
			if(!isClosed){
				numberOfOpenIssues = numberOfOpenIssues + 1;
			}
		}
		if(allBugsResponse.get("bugs") == null || numberOfOpenIssues == 0){
			payload = Map();
			payload.put("title",event);
			createBugResponse = invokeurl
			[
				url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/"
				type :POST
				parameters:payload
				connection:"projectxmail"
			];
			id = createBugResponse.get("bugs").get(0).get("id");
			payload = Map();
			issueValue = "IP=" + ip +",process=" + process + ",processName="+processName+",eventLink="+eventLink;
			payload.put("content",issueValue);

			commentResponse = invokeurl
			[
				url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
				type :POST
				parameters:payload
				connection:"projectxmail"
			];
			payload = Map();
			payload.put("tag_id",SERVER_PERFORMACE_TAG_ID);
			payload.put("entity_id",id);
			payload.put("entityType",6);
			createTagResponse = invokeurl
			[
				url :"https://projectsapi.zoho.in/api/v3/portal/"+portalId+"/projects/"+projectId+"/tags/associate"
				type : POST
				parameters : payload
				connection:"projectxmail"
			];
		}
		else{
			isIssueCreated = false;
			for each bug in allBugsResponse.get("bugs")
			{
				id = bug.get("id");
				title = bug.get("title");
			
				if(title.equals(event) && (!bug.get("closed"))){
					getCommentForBug = invokeurl
					[
						url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
						type :GET
						connection:"projectxmail"
					];
					commentCreatedTime = getCommentForBug.get("comments").get(0).get("created_time_format");
					total_hours = hoursBetween(commentCreatedTime,now);
					if(total_hours < 1){
						
						payload = Map();
						issueValue = "IP=" + ip +",process=" + process + ",processName="+processName+",eventLink="+eventLink;
						payload.put("content",issueValue);
					
						commentResponse = invokeurl
						[
							url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
							type :POST
							parameters:payload
							connection:"projectxmail"
						];
						payload = Map();
						payload.put("tag_id",SERVER_PERFORMACE_TAG_ID);
						payload.put("entity_id",id);
						payload.put("entityType",6); // bug type
						createTagResponse = invokeurl
						[
							url :"https://projectsapi.zoho.in/api/v3/portal/"+portalId+"/projects/"+projectId+"/tags/associate"
							type : POST
							parameters : payload
							connection:"projectxmail"
						];
						isIssueCreated = true;
						break;
					}
				}
				
			}
		
			if(!isIssueCreated){
				payload = Map();
				payload.put("title",event);
				createBugResponse = invokeurl
				[
					url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/"
					type :POST
					parameters:payload
					connection:"projectxmail"
				];
				id = createBugResponse.get("bugs").get(0).get("id");
				payload = Map();
				issueValue = "IP=" + ip +",process=" + process + ",processName="+processName+",eventLink="+eventLink;
				payload.put("content",issueValue);

				commentResponse = invokeurl
				[
					url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
					type :POST
					parameters:payload
					connection:"projectxmail"
				];
				payload = Map();
				payload.put("tag_id",SERVER_PERFORMACE_TAG_ID);
				payload.put("entity_id",id);
				payload.put("entityType",6);
				createTagResponse = invokeurl
				[
					url :"https://projectsapi.zoho.in/api/v3/portal/"+portalId+"/projects/"+projectId+"/tags/associate"
					type : POST
					parameters : payload
					connection:"projectxmail"
				];
			}
				

		}
	}
}


if(MYSQL_MASTER_SLOW_QUERIES > 0){
	counter = leftpad("1",MYSQL_MASTER_SLOW_QUERIES).replaceAll(" ","1,").toList();
	newText = text;
	for each iterator in counter {
		ip = "";
		event = "";
		eventLink = "";
		process = "";
		processName = "";
		domain = "";
		eventExtension = "";
		startIndex = text.find("MYSQL_MASTER_SLOW_QUERIES&");
		newStartIndex = startIndex - 2200;
		endIndex = startIndex + 1540;
		newText = text.substring(newStartIndex, endIndex);

		if(newStartIndex < 0){
			newStartIndex = 0;
		}
		if(endIndex > text.length()){
			endIndex = text.length();
		}

		if(newText.contains("us4-mi.zohodcm")){
			domain = "us4-mi.zohodcm.com";
			eventExtension = "US4";
		}
		else if(newText.contains("in2-mi.zohodcm")){
			domain = "in2-mi.zohodcm.com";
			eventExtension = "IN2";
		}
		else if(newText.contains("ca1-mi.zohodcm")){
			domain = "ca1-mi.zohodcm.com";
			eventExtension = "CA1";
		}
		else if(newText.contains("cn2-mi.zohodcm")){
			domain = "cn2-mi.zohodcm.com";
			eventExtension = "CN2";
		}
		else if(newText.contains("eu1-mi.zohodcm")){
			domain = "eu1-mi.zohodcm.com";
			eventExtension = "EU1";
		}
		else if(newText.contains("ca1-mi.zohodcm")){
			domain = "ca1-mi.zohodcm.com";
			eventExtension = "CA1";
		}
		else if(newText.contains("jp1-mi.zohodcm")){
			domain = "jp1-mi.zohodcm.com";
			eventExtension = "JP1";
		}
		else if(newText.contains("in2nic-mi.zohodcm")){
			domain = "in2nic-mi.zohodcm.com";
			eventExtension = "IN2NIC";
		}
		else if(newText.contains("sa1-mi.zohodcm")){
			domain = "sa1-mi.zohodcm.com";
			eventExtension = "SA1";
		}
		else if(newText.contains("uk1-mi.zohodcm")){
			domain = "uk1-mi.zohodcm.com";
			eventExtension = "UK1";
		}
		else if(newText.contains("au1-mi.zohodcm")){
			domain = "au1-mi.zohodcm.com";
			eventExtension = "AU1";
		}
		else if(newText.contains("us4charm-mi.zohodcm")){
			domain = "us4charm-mi.zohodcm.com";
			eventExtension = "US4Charm";
		}

		if(newText.contains("time="))
		{
			startIndex = newText.find("time=");
			endIndex = startIndex + 200;
			list = newText.substring(startIndex,endIndex).toList("");
			url = "";
			for each  ch in list
			{
				if(ch == "&")
				{
					break;
				}
				url = url + ch;
			}
			modifiedText = newText.replaceAll(url,"");
			newText = modifiedText;
			eventLink = "<a href='https://"+domain+"/serviceMap.do?serviceName=Chat&'"+url+">https://"+domain+"/serviceMap.do?serviceName=Chat&"+url+"</a>";
		}
		
		if(newText.contains("IP&")){
			ipStartIndex = newText.find("IP&") + 39;
			ipEndIndex = ipStartIndex + 14;
			unFormattedIpList = newText.subString(ipStartIndex,ipEndIndex).toList("");
			for each  ch in unFormattedIpList
			{
				if(ch == '&')
				{
					break;
				}
				ip = ip + ch;
			}
		}
	
	

		if(newText.contains("Event&"))
		{
			eventStartIndex = newText.find("Event&") + 270;
			eventEndIndex = eventStartIndex + 30;
			// appx max characters length
			unFormattedEventList = newText.subString(eventStartIndex,eventEndIndex).toList("");
			for each  ch in unFormattedEventList
			{
				if(ch == '&')
				{
					break;
				}
				event = event + ch;
			}
			event+="_"+eventExtension;
		}
		
		if(newText.contains("Process&") && (newText.find("Process&")))
		{
			processStartIndex = newText.find("Process&") + 109;
			processEndIndex = processStartIndex + 40;
			// appx max characters length
			unFormattedProcessList = newText.subString(processStartIndex,processEndIndex).toList("");
			for each  ch in unFormattedProcessList
			{
				if(ch == '&')
				{
					break;
				}
				process = process + ch;
			}
		}
		else if(newText.contains("Process Name&"))
		{
			processNameStartIndex = newText.find("Process Name&") + 114;
			processNameEndIndex = processNameStartIndex + 30;
			// appx max characters length
			unFormattedProcessNameList = newText.subString(processNameStartIndex,processNameEndIndex).toList("");
			for each  ch in unFormattedProcessNameList
			{
				if(ch == '&')
				{
					break;
				}
				processName = processName + ch;
			}
		}



		text = text.replaceFirst("MYSQL_MASTER_SLOW_QUERIES&","MYSQL__MASTER__SLOW__QUERIES&");

		allBugsResponse = invokeurl
		[
			url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/"
			type :GET
			connection:"projectxmail"
		];
		numberOfOpenIssues = 0;
		for each bug in allBugsResponse.get("bugs") {
			isClosed = bug.get("closed");
			if(!isClosed){
				numberOfOpenIssues = numberOfOpenIssues + 1;
			}
		}

		if(allBugsResponse.get("bugs") == null || numberOfOpenIssues == 0){
			payload = Map();
			payload.put("title",event);
			createBugResponse = invokeurl
			[
				url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/"
				type :POST
				parameters:payload
				connection:"projectxmail"
			];
			id = createBugResponse.get("bugs").get(0).get("id");
			payload = Map();
			issueValue = "IP=" + ip +",process=" + process + ",processName="+processName+",eventLink="+eventLink;
			payload.put("content",issueValue);

			commentResponse = invokeurl
			[
				url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
				type :POST
				parameters:payload
				connection:"projectxmail"
			];
			payload = Map();
			payload.put("tag_id",SERVER_PERFORMACE_TAG_ID);
			payload.put("entity_id",id);
			payload.put("entityType",6);
			createTagResponse = invokeurl
			[
				url :"https://projectsapi.zoho.in/api/v3/portal/"+portalId+"/projects/"+projectId+"/tags/associate"
				type : POST
				parameters : payload
				connection:"projectxmail"
			];
		}
		else{
			isIssueCreated = false;
			for each bug in allBugsResponse.get("bugs")
			{
				id = bug.get("id");
				title = bug.get("title");
				if(title.equals(event) && (!bug.get("closed"))){
					getCommentForBug = invokeurl
					[
						url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
						type :GET
						connection:"projectxmail"
					];
					commentCreatedTime = getCommentForBug.get("comments").get(0).get("created_time_format");
					total_hours = hoursBetween(commentCreatedTime,now);
					if(total_hours < 1){
						payload = Map();
						issueValue = "IP=" + ip +",process=" + process + ",processName="+processName+",eventLink="+eventLink;
						payload.put("content",issueValue);
					
						commentResponse = invokeurl
						[
							url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
							type :POST
							parameters:payload
							connection:"projectxmail"
						];
						payload = Map();
						payload.put("tag_id",SERVER_PERFORMACE_TAG_ID);
						payload.put("entity_id",id);
						payload.put("entityType",6);
						createTagResponse = invokeurl
						[
							url :"https://projectsapi.zoho.in/api/v3/portal/"+portalId+"/projects/"+projectId+"/tags/associate"
							type : POST
							parameters : payload
							connection:"projectxmail"
						];
						isIssueCreated = true;
						break;
					}
				}
				
			}
		
			if(!isIssueCreated){
				payload = Map();
				payload.put("title",event);
				createBugResponse = invokeurl
				[
					url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/"
					type :POST
					parameters:payload
					connection:"projectxmail"
				];
				id = createBugResponse.get("bugs").get(0).get("id");
				payload = Map();
				issueValue = "IP=" + ip +",process=" + process + ",processName="+processName+",eventLink="+eventLink;
				payload.put("content",issueValue);

				commentResponse = invokeurl
				[
					url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
					type :POST
					parameters:payload
					connection:"projectxmail"
				];
				payload = Map();
				payload.put("tag_id",SERVER_PERFORMACE_TAG_ID);
				payload.put("entity_id",id);
				payload.put("entityType",6);
				createTagResponse = invokeurl
				[
					url :"https://projectsapi.zoho.in/api/v3/portal/"+portalId+"/projects/"+projectId+"/tags/associate"
					type : POST
					parameters : payload
					connection:"projectxmail"
				];
			}
				

		}
	}
}

if(PROCESS_DOWN > 0){
	
	counter = leftpad("1",PROCESS_DOWN).replaceAll(" ","1,").toList();
	newText = text;
	for each iterator in counter {
		ip = "";
		event = "";
		eventLink = "";
		process = "";
		processName = "";
		domain = "";
		eventExtension = "";
		startIndex = text.find("PROCESS_DOWN&");
		newStartIndex = startIndex - 1900;
		endIndex = startIndex + 1659;
		newText = text.substring(newStartIndex, endIndex);

		if(newStartIndex < 0){
			newStartIndex = 0;
		}
		if(endIndex > text.length()){
			endIndex = text.length();
		}

		if(newText.contains("us4-mi.zohodcm")){
			domain = "us4-mi.zohodcm.com";
			eventExtension = "US4";
		}
		else if(newText.contains("in2-mi.zohodcm")){
			domain = "in2-mi.zohodcm.com";
			eventExtension = "IN2";
		}
		else if(newText.contains("ca1-mi.zohodcm")){
			domain = "ca1-mi.zohodcm.com";
			eventExtension = "CA1";
		}
		else if(newText.contains("cn2-mi.zohodcm")){
			domain = "cn2-mi.zohodcm.com";
			eventExtension = "CN2";
		}
		else if(newText.contains("eu1-mi.zohodcm")){
			domain = "eu1-mi.zohodcm.com";
			eventExtension = "EU1";
		}
		else if(newText.contains("ca1-mi.zohodcm")){
			domain = "ca1-mi.zohodcm.com";
			eventExtension = "CA1";
		}
		else if(newText.contains("jp1-mi.zohodcm")){
			domain = "jp1-mi.zohodcm.com";
			eventExtension = "JP1";
		}
		else if(newText.contains("in2nic-mi.zohodcm")){
			domain = "in2nic-mi.zohodcm.com";
			eventExtension = "IN2NIC";
		}
		else if(newText.contains("sa1-mi.zohodcm")){
			domain = "sa1-mi.zohodcm.com";
			eventExtension = "SA1";
		}
		else if(newText.contains("uk1-mi.zohodcm")){
			domain = "uk1-mi.zohodcm.com";
			eventExtension = "UK1";
		}
		else if(newText.contains("au1-mi.zohodcm")){
			domain = "au1-mi.zohodcm.com";
			eventExtension = "AU1";
		}
		else if(newText.contains("us4charm-mi.zohodcm")){
			domain = "us4charm-mi.zohodcm.com";
			eventExtension = "US4Charm";
		}

		if(newText.contains("time="))
		{
			startIndex = newText.find("time=");
			endIndex = startIndex + 200;
			list = newText.substring(startIndex,endIndex).toList("");
			url = "";
			for each  ch in list
			{
				if(ch == "&")
				{
					break;
				}
				url = url + ch;
			}
			modifiedText = newText.replaceAll(url,"");
			newText = modifiedText;
			eventLink = "<a href='https://"+domain+"/serviceMap.do?serviceName=Chat&'"+url+">https://"+domain+"/serviceMap.do?serviceName=Chat&"+url+"</a>";
		}
		
		if(newText.contains("IP&")){
			ipStartIndex = newText.find("IP&") + 39;
			ipEndIndex = ipStartIndex + 14;
			unFormattedIpList = newText.subString(ipStartIndex,ipEndIndex).toList("");
			for each  ch in unFormattedIpList
			{
				if(ch == '&')
				{
					break;
				}
				ip = ip + ch;
			}
		}
	
	

		if(newText.contains("Event&"))
		{
			eventStartIndex = newText.find("Event&") + 270;
			eventEndIndex = eventStartIndex + 30;
			// appx max characters length
			unFormattedEventList = newText.subString(eventStartIndex,eventEndIndex).toList("");
			for each  ch in unFormattedEventList
			{
				if(ch == '&')
				{
					break;
				}
				event = event + ch;
			}
			event+="_"+eventExtension;
		}
		
		if(newText.contains("Process&") && (newText.find("Process&")))
		{
			processStartIndex = newText.find("Process&") + 109;
			processEndIndex = processStartIndex + 40;
			// appx max characters length
			unFormattedProcessList = newText.subString(processStartIndex,processEndIndex).toList("");
			for each  ch in unFormattedProcessList
			{
				if(ch == '&')
				{
					break;
				}
				process = process + ch;
			}
		}
		else if(newText.contains("Process Name&"))
		{
			processNameStartIndex = newText.find("Process Name&") + 114;
			processNameEndIndex = processNameStartIndex + 30;
			// appx max characters length
			unFormattedProcessNameList = newText.subString(processNameStartIndex,processNameEndIndex).toList("");
			for each  ch in unFormattedProcessNameList
			{
				if(ch == '&')
				{
					break;
				}
				processName = processName + ch;
			}
		}


		text = text.replaceFirst("PROCESS_DOWN&","PROCESS__DOWN&");

		allBugsResponse = invokeurl
		[
			url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/"
			type :GET
			connection:"projectxmail"
		];
		numberOfOpenIssues = 0;
		for each bug in allBugsResponse.get("bugs") {
			isClosed = bug.get("closed");
			if(!isClosed){
				numberOfOpenIssues = numberOfOpenIssues + 1;
			}
		}
		if(allBugsResponse.get("bugs") == null || numberOfOpenIssues == 0){
			payload = Map();
			payload.put("title",event);
			createBugResponse = invokeurl
			[
				url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/"
				type :POST
				parameters:payload
				connection:"projectxmail"
			];
			id = createBugResponse.get("bugs").get(0).get("id");
			payload = Map();
			issueValue = "IP=" + ip +",process=" + process + ",processName="+processName+",eventLink="+eventLink;
			payload.put("content",issueValue);

			commentResponse = invokeurl
			[
				url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
				type :POST
				parameters:payload
				connection:"projectxmail"
			];
			payload = Map();
			payload.put("tag_id",SERVER_PERFORMACE_TAG_ID);
			payload.put("entity_id",id);
			payload.put("entityType",6);
			createTagResponse = invokeurl
			[
				url :"https://projectsapi.zoho.in/api/v3/portal/"+portalId+"/projects/"+projectId+"/tags/associate"
				type : POST
				parameters : payload
				connection:"projectxmail"
			];
		}
		else{
		
			isIssueCreated = false;
			for each bug in allBugsResponse.get("bugs")
			{
				id = bug.get("id");
				title = bug.get("title"); 
				if(title.equals(event) && (!bug.get("closed"))){
					getCommentForBug = invokeurl
					[
						url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
						type :GET
						connection:"projectxmail"
					];
					commentCreatedTime = getCommentForBug.get("comments").get(0).get("created_time_format");
					total_hours = hoursBetween(commentCreatedTime,now);
					if(total_hours < 1){
	
						payload = Map();
						issueValue = "IP=" + ip +",process=" + process + ",processName="+processName+",eventLink="+eventLink;
						payload.put("content",issueValue);
					
						commentResponse = invokeurl
						[
							url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
							type :POST
							parameters:payload
							connection:"projectxmail"
						];
						payload = Map();
						payload.put("tag_id",SERVER_PERFORMACE_TAG_ID);
						payload.put("entity_id",id);
						payload.put("entityType",6);
						createTagResponse = invokeurl
						[
							url :"https://projectsapi.zoho.in/api/v3/portal/"+portalId+"/projects/"+projectId+"/tags/associate"
							type : POST
							parameters : payload
							connection:"projectxmail"
						];
			
						isIssueCreated = true;
						break;
					}
				}
				
			}
		
			if(!isIssueCreated){
				payload = Map();
				payload.put("title",event);
				createBugResponse = invokeurl
				[
					url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/"
					type :POST
					parameters:payload
					connection:"projectxmail"
				];
				id = createBugResponse.get("bugs").get(0).get("id");
				payload = Map();
				issueValue = "IP=" + ip +",process=" + process + ",processName="+processName+",eventLink="+eventLink;
				payload.put("content",issueValue);

				commentResponse = invokeurl
				[
					url :"https://projectsapi.zoho.in/restapi/portal/" + portalId + "/projects/" + projectId + "/bugs/" + id + "/comments/"
					type :POST
					parameters:payload
					connection:"projectxmail"
				];
				payload = Map();
				payload.put("tag_id",SERVER_PERFORMACE_TAG_ID);
				payload.put("entity_id",id);
				payload.put("entityType",6);
				createTagResponse = invokeurl
				[
					url :"https://projectsapi.zoho.in/api/v3/portal/"+portalId+"/projects/"+projectId+"/tags/associate"
					type : POST
					parameters : payload
					connection:"projectxmail"
				];
			}
				

		}
	}
}
