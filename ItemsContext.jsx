import { createContext, useContext, useState } from "react";

export const ItemContext = createContext();

export const ItemProvider = ({ children }) => {
  const [items] = useState([
    { id: 1, title: "IT Industry Growth in 2025", description: "How tech companies are scaling globally.", image: "/placeholder.png" },
    { id: 2, title: "Cloud Security Essentials", description: "A simple guide to keeping data safe.", image: "/placeholder.png" },
    { id: 3, title: "AI in Modern Business", description: "How companies use AI to boost efficiency.", image: "/placeholder.png" },
    { id: 4, title: "Top 10 UI/UX Trends", description: "What designs will dominate 2025.", image: "/placeholder.png" },
    { id: 5, title: "Cybersecurity Basics", description: "How beginners should protect their accounts.", image: "/placeholder.png" },
    { id: 6, title: "What is DevOps?", description: "A simple intro to pipeline automation.", image: "/placeholder.png" },
    { id: 7, title: "Frontend vs Backend", description: "Which direction to choose?", image: "/placeholder.png" },
    { id: 8, title: "React Best Practices", description: "Clean code rules for modern React.", image: "/placeholder.png" },
    { id: 9, title: "Big Data Explained", description: "Why companies collect data and how they use it.", image: "/placeholder.png" },
    { id: 10, title: "AI Ethics", description: "Why regulating AI matters.", image: "/placeholder.png" },
    { id: 11, title: "Tech Careers", description: "How to start a career in IT with no experience.", image: "/placeholder.png" },
    { id: 12, title: "Machine Learning Basics", description: "A simple explanation for beginners.", image: "/placeholder.png" },
    { id: 13, title: "Servers & Hosting", description: "How websites live on the internet.", image: "/placeholder.png" },
    { id: 14, title: "Blockchain Uses", description: "Where blockchain actually helps.", image: "/placeholder.png" },
    { id: 15, title: "IT Project Management", description: "How tech projects stay on track.", image: "/placeholder.png" },
  ]);

  return <ItemContext.Provider value={{ items }}>{children}</ItemContext.Provider>;
};

export const useItems = () => useContext(ItemContext);
