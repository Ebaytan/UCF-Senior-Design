//
//  BackTableVC.swift
//  KnightsCoffee
//
//  Created by Evan Baytan on 2/24/16.
//  Copyright © 2016 Ebaytan. All rights reserved.
//

import Foundation

class BackTableVC: UITableViewController {
    
    
    var TableArray = [String]()
    
    override func viewDidLoad() {
        TableArray = ["Knights Coffee", "Sign Up", "History", "Favorites", "Settings"]
    }
    
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return TableArray.count
    }
    
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("cell", forIndexPath: indexPath) as UITableViewCell
        
        cell.textLabel?.text = TableArray[indexPath.row]
        
        return cell
    }
}