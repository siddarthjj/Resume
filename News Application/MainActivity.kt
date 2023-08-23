package com.example.sid1

import android.annotation.SuppressLint
import android.net.Uri
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.browser.customtabs.CustomTabsIntent
import androidx.recyclerview.widget.LinearLayoutManager
import com.android.volley.Request
import com.android.volley.toolbox.JsonObjectRequest
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity(), NewsItemClicked {

    private lateinit var mAdapter: NewsListAdapter //creating instance of NewsListAdapter // late lateinit used to declare those variables that are guaranteed to be initialized in the future

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
//        val recyclerview = findViewById<RecyclerView>(R.id.recyclerview)
        recyclerView.layoutManager = LinearLayoutManager(this) //to tell which layout this is
        fetchData()
        mAdapter = NewsListAdapter(this) // putting items in the adapter
        recyclerView.adapter = mAdapter // linking adapter and recyclerView
    }
    @SuppressLint("SuspiciousIndentation")
    private fun fetchData(){ // to get data from api
       val url = "https://newsdata.io/api/1/news?apikey=pub_1527100303ea8d96c01fdfd09dd8834599ccd&language=en&country=us,gb,in"
//        val url = "https://newsdata.io/api/1/news?apikey=YOUR_API_KEY&language=en"


        val jsonObjectRequest = JsonObjectRequest(
            Request.Method.GET, //get request is for getting data
            url,
            null,
            //response .listener
            {
                val newsJsonArray = it.getJSONArray("results") //results is json array
                val newsArray = ArrayList<News>() //list of news
                for(i in 0 until newsJsonArray.length()) { //to iterate through json objects
                    val newsJsonObject = newsJsonArray.getJSONObject(i) //taking each json object
                    val news = News( //news object , and passing the constructor
                        newsJsonObject.getString("title"),
//                        newsJsonObject.getString("language"),
                        newsJsonObject.getString("link"),
                        newsJsonObject.getString("image_url")
                    )
                    newsArray.add(news)
                }
                mAdapter.updateNews(newsArray) // passing the array to adapter
            },
            {
                //error is thrown only when there is an internet error
            }
        )
// Access the RequestQueue through your singleton class.
                    MySingleton.getInstance (this).addToRequestQueue(jsonObjectRequest)


    }

    override fun onItemClicked(item: News) {
        val builder = CustomTabsIntent.Builder()
        val customTabsIntent = builder.build()
        customTabsIntent.launchUrl(this, Uri.parse(item.link))
    }
}