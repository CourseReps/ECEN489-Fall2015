<html>

<head>
	<title>Reading JSON with jQuery</title>
	<link rel="stylesheet" href="css/jquery.lightbox-0.5.css" type="text/css" media="screen,projection" />
	<style type="text/css">
		body {
		  font-size:14px;
		}
		h1 {
		  display:block;
		  margin:10px auto 20px;
		  font:24px/30px georgia, serif;
		  text-align:center;
		  letter-spacing:.06em;
		  color:#222;
		}
		#container {
		  width:750px;
		  margin:10px auto;
		}
		#user-wrap {
		  width:340px;
		  float:left;
		}
		h2 {
		  font:18px/30px georgia, serif;
		  letter-spacing:.06em;
		}
		#user-wrap p {
		  width:320px;
		}
		#feed {
		  float:left;
		  width:308px;
		  height:308px;
		  margin:10px auto;
		  border:4px solid #222;
		  background:#FFF;
		  overflow:hidden;
		}
		#feed > span {
		  display:block;
		  margin:130px auto;
		  padding:0;
		  text-align:center;
		}
		#feed > a > img {
		  border:1px solid #DDD;
		}
		p, pre {
		  width:960px;
		  margin:10px auto;
		  color:#444;
		  clear:both;
		}
		p#feed {
		  clear:none;
		}
		p {
		  letter-spacing:.04em;
		}
		pre {
		  border:1px solid #444;
		  padding:10px;
		  background:#FCFCFC;
		}
	</style>
</head>

<body>

	<h1> Using jQuery to Read a JSON Feed </h1>

	<div id="container">
		<div id="user-wrap">
			<h2> Select a Person to See Their Photos &#187; </h2>
			<ul id="users"></ul>
			<p>
				Thanks to <a href="https://www.flickr.com/photos/prashkan/">Prashanth</a> for 
				letting me use his photostream in this demo!
			</p>
		</div>
		<p id="feed"></p>
	</div>

	<p>
		The photos above are loaded by calling this function:
	</p>
	
<pre><code class="js">function loadFlickr(flickrid)
{


	// Request the JSON and process it
	$.ajax({
		type:&#39;GET&#39;,
		url:&quot;http://api.flickr.com/services/feeds/photos_public.gne&quot;,
		data:&quot;id=&quot;+flickrid+&quot;&amp;lang=en-us&amp;format=json&amp;jsoncallback=?&quot;,
		success:function(feed) {
			// Create an empty array to store images
			var thumbs = [];

			// Loop through the items
			for(var i=0, l=feed.items.length; i &lt; l &amp;&amp; i &lt; 16; ++i) 
			{
				// Manipulate the image to get thumb and medium sizes
				var img = feed.items[i].media.m.replace(
					/^(.*?)_m\.jpg$/, 
					&#39;&lt;a href=&quot;$1.jpg&quot;&gt;&lt;img src=&quot;$1_s.jpg&quot; alt=&quot;&quot; /&gt;&lt;/a&gt;&#39;
				);

				// Add the new element to the array
				thumbs.push(img);
			}

			// Display the thumbnails on the page
			$(&#39;#feed&#39;).html(thumbs.join(&#39;&#39;));

			// A function to add a lightbox effect
			addLB();
		},
		dataType:&#39;jsonp&#39;
	});
}</code></pre>

	
	<script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js"></script>
	<script type="text/javascript">
		var people = ([{
			"name":"Prashanth Kannan", 
			"id":"25776249@N06"
		},
		]);

		$(document).ready(function() {
			for(i=0, l=people.length; i<l; ++i)
			{
				$('#users').append(
					'<li><a href="javascript:loadFlickr(\''
						+people[i].id+'\')">'+people[i].name+"</a></li>\n"
				);
			}
			loadFlickr("25776249@N06");
		});

		function loadFlickr(flickrid)
		{
			// Display a loading icon in our display element
			$('#feed').html('<span><img src="images/lightbox-ico-loading.gif" /></span>');
		
			// Request the JSON and process it
			$.ajax({
				type:'GET',
				url:"http://api.flickr.com/services/feeds/photos_public.gne",
				data:"id="+flickrid+"&lang=en-us&format=json&jsoncallback=?",
				success:function(feed) {
					// Create an empty array to store images
					var thumbs = [];

					// Loop through the items
					for(var i=0, l=feed.items.length; i < l && i < 16; ++i) 
					{
						// Manipulate the image to get thumb and medium sizes
						var img = feed.items[i].media.m.replace(
							/^(.*?)_m\.jpg$/, 
							'<a href="$1.jpg"><img src="$1_s.jpg" alt="" /></a>'
						);
		
						// Add the new element to the array
						thumbs.push(img);
					}
		
					// Display the thumbnails on the page
					$('#feed').html(thumbs.join(''));
		
					// A function to add a lightbox effect
					addLB();
				},
				dataType:'jsonp'
			});
		}
	</script>

	<!-- Syntax highlighting and lightbox -->

    <script src="/_scripts/jquery.chili-2.2.js" type="text/javascript"></script>
    <script src="/_scripts/chili/recipes.js" type="text/javascript"></script>
    <script type="text/javascript">
        ChiliBook.replaceSpace = "&nbsp;";
    </script>

	<script type="text/javascript" src="js/jquery.lightbox-0.5.min.js"></script>
	<script type="text/javascript">
		function addLB() {
			$('#feed a').lightBox();
		}
	</script>


</body>

</html>