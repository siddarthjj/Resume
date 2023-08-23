package com.example.sid1

import android.annotation.SuppressLint
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide

class NewsListAdapter(private val listener: NewsItemClicked) : RecyclerView.Adapter<NewsViewHolder>() { //adapter gets data from NewsItemClicked , listener is an instance of NewsItemClicked
    private val items: ArrayList<News> = ArrayList()  //array list of news
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): NewsViewHolder { //calls when viewHolder is created
        val view = LayoutInflater.from(parent.context).inflate(R.layout.item_news, parent , false) //item_news to be converted to java/view
        val viewHolder = NewsViewHolder(view)
        view.setOnClickListener{
            listener.onItemClicked(items[viewHolder.adapterPosition]) //since we dont want handle in the adapter we use interface
        }
        return viewHolder //returns a viewholder , taking instance from the NewsViewHolder class
    }

    override fun onBindViewHolder(holder: NewsViewHolder, position: Int) { // this binds the data and view
        val currentItem = items[position]
        holder.titleView.text = currentItem.title // each is put inside the view holder
        Glide.with(holder.itemView.context).load(currentItem.image_url).into(holder.image) //this is for image binding , load for url of image , into is image id
    }

    override fun getItemCount(): Int { // called once and tells how many items are present in the view
        return items.size
    }

     @SuppressLint("NotifyDataSetChanged")
     fun  updateNews(updatedNews: ArrayList<News>) //update news
     {
         items.clear()
         items.addAll(updatedNews)

         notifyDataSetChanged() //when this is called , onCreateViewHolder ,onBindViewHolder and getItemCount is called all over again
     }
}

class NewsViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) { //itemView has item_news.xml
    val titleView: TextView = itemView.findViewById(R.id.title)
    val image: ImageView =itemView.findViewById(R.id.image)
}

interface NewsItemClicked
{
    fun onItemClicked(item : News) //function item of type News
}